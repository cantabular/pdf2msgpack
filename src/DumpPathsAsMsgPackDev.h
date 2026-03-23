#include <iostream>
#include <sstream>
#include <string>

#include <msgpack.hpp>

// /usr/include/poppler/...
#include <GfxFont.h>
#include <GfxState.h>
#include <OutputDev.h>

#include "util.hpp"

const int EO_FILL = 10;
const int STROKE = 11;
const int FILL = 12;
const int SET_STROKE_COLOR = 13;
const int SET_STROKE_WIDTH = 14;
const int SET_FILL_COLOR = 15;

typedef struct { double x, y; } Point;

class Mat2x3 {
  const std::array<double, 6> &m;

public:
  Mat2x3(const std::array<double, 6> &m) : m(m) {}

  inline const Point mul(const double x, const double y) const {
    auto xt = x * m[0] + y * m[2] + m[4];
    auto yt = x * m[1] + y * m[3] + m[5];
    return Point{xt, yt};
  }
};

class PathPoint {
public:
  PathPoint(double x, double y) : line(x, y), is_curve(false) {}
  PathPoint(double a, double b, double c, double d, double e, double f)
      : curve(a, b, c, d, e, f), is_curve(true) {}

  std::tuple<double, double> line;
  std::tuple<double, double, double, double, double, double> curve;

  bool is_curve;

  void msgpack_pack(msgpack::packer<std::basic_ostream<char>> &pk) const {
    if (is_curve) {
      pk.pack(curve);
    } else {
      pk.pack(line);
    }
  }
};

bool operator!=(const GfxRGB &a, const GfxRGB &b) {
  return !(a.r == b.r && a.g == b.g && a.b == b.b);
}

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
  namespace adaptor {

  template <> struct pack<GfxRGB> {
    template <typename Stream>
    msgpack::packer<Stream> &operator()(msgpack::packer<Stream> &p,
                                        GfxRGB const &color) const {
      auto c = [](const GfxColorComp x) -> uint8_t {
        // 255 * x + 0.5  =  256 * x - x + 0x8000
        return static_cast<uint8_t>(((x << 8) - x + 0x8000) >> 16);
      };
      p.pack_array(3);
      p.pack_fix_uint8(c(color.r));
      p.pack_fix_uint8(c(color.g));
      p.pack_fix_uint8(c(color.b));
      return p;
    }
  };

  } // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

class DumpPathsAsMsgPackDev : public OutputDev {
public:
  DumpPathsAsMsgPackDev() : packer(buffer), item_count(0) {}

  std::ostringstream buffer;
  msgpack::packer<std::ostream> packer;

  // Number of items written to the stream.
  int item_count;

  GfxRGB prevFillRGB = {0, 0, 0}, prevStrokeRGB = {0, 0, 0};
  double prevStrokeWidth = -1;

public:
  // Writes the packed path information to a stream.
  //
  // The `packer` is initialized with a `buffer`
  // when an instance of `DumpAsMsgPackDev` is created.
  // The path information is written to the `packer` and
  // the packed data is in this method streamed to `out`.
  void pack(std::ostream &out) {
    msgpack::packer<std::ostream>(out).pack_array(item_count);
    out << buffer.str();
  }

  bool upsideDown() { return true; }
  bool useDrawChar() { return true; }
  bool interpretType3Chars() { return true; }

  void eoFill(GfxState *state) {
    doPath(state, state->getCTM(), state->getPath(), EO_FILL);
  }

  void stroke(GfxState *state) {
    doPath(state, state->getCTM(), state->getPath(), STROKE);
  }

  void fill(GfxState *state) {
    doPath(state, state->getCTM(), state->getPath(), FILL);
  }

  void recordStateChanges(GfxState *state, int path_type) {
    switch (path_type) {

    case FILL:
    case EO_FILL:
      GfxRGB curFillRGB;
      state->getFillRGB(&curFillRGB);
      if (prevFillRGB != curFillRGB) {
        prevFillRGB = curFillRGB;
        packer.pack_array(2);
        packer.pack_fix_uint8(SET_FILL_COLOR);
        packer.pack(curFillRGB);
        item_count++;
      }

    // break;
    // Fallthrough. Strokes apply to fills, too.

    case STROKE:
      GfxRGB curStrokeRGB;
      state->getStrokeRGB(&curStrokeRGB);
      if (prevStrokeRGB != curStrokeRGB) {
        prevStrokeRGB = curStrokeRGB;
        packer.pack_array(2);
        packer.pack_fix_uint8(SET_STROKE_COLOR);
        packer.pack(curStrokeRGB);
        item_count++;
      }

      double curStrokeWidth;
      curStrokeWidth = state->getLineWidth();
      if (prevStrokeWidth != curStrokeWidth) {
        prevStrokeWidth = curStrokeWidth;
        packer.pack(std::make_tuple(SET_STROKE_WIDTH, curStrokeWidth));
        item_count++;
      }

      break;

    default:
      std::cerr << "unknown path type: " << path_type << std::endl;
    }
  }

  void doPath(GfxState *state, const Mat2x3 &transform, const GfxPath *path,
              int path_type) {
    recordStateChanges(state, path_type);

    auto n = path->getNumSubpaths();

    for (auto i = 0; i < n; ++i) {
      auto subpath = path->getSubpath(i);
      auto m = subpath->getNumPoints();
      auto j = 0;

      std::vector<PathPoint> path_points;
      while (j < m) {
        // Consider removing the bounds end check: j < m - 2
        // This should not be necessary with the j += 2 increment,
        // but is an extra safeguard to ensure we do not end up accessing
        // the uninitialised part of arrays.
        // If the number of points, m, is, say, 10,
        // we need to stop at point 8 to ensure we don't exceed point 10
        // because we access the second value beyond 8.
        // This actually corresponds to j = 7 as j is 0-indexed,
        // that is, j must be less than m - 2.
        if (subpath->getCurve(j) && (j < m - 2)) {
          auto a = transform.mul(subpath->getX(j + 0), subpath->getY(j + 0)),
               b = transform.mul(subpath->getX(j + 1), subpath->getY(j + 1)),
               c = transform.mul(subpath->getX(j + 2), subpath->getY(j + 2));

          path_points.push_back(PathPoint(a.x, a.y, b.x, b.y, c.x, c.y));
          // Consider replacing this with j += 3 in future.
          // See the associated commit message or #154 for a full explanation.
          // Poppler's own code iterates using j += 3 for subpath curves.
          //
          // This is a hack to keep the behaviour of pdf2msgpack close to what it was,
          // but with reproducible output.
          // The current result is that the first point of the curve gets acted on as previously,
          // the second point of the curve no longer incorrectly adds another curve point,
          // but the final point does get included as a standalone point
          // (the final point also gets handled by the else block below).
          // Including the final point as a standalone point may not be strictly correct,
          // but more closely retains the previous behaviour.
          j += 2;
        } else {
          auto x = subpath->getX(j), y = subpath->getY(j);

          auto t = transform.mul(x, y);
          path_points.push_back(PathPoint(t.x, t.y));
          ++j;
        }
      }

      if (!path_points.empty()) {
        if (subpath->isClosed()) {
          path_points.push_back(path_points[0]);
        }
        packer.pack(std::make_tuple(path_type, path_points));
        item_count++;
      }
    }
  }
};
