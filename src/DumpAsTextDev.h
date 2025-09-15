// /usr/include/poppler/...
#include <GfxFont.h>
#include <GfxState.h>
#include <OutputDev.h>

#include "util.hpp"

// Automatically generated, lists all functions and their prototypes.
// (Or see poppler/OutputDev.h).
class DumpAsTextDev : public OutputDev {
public:
  bool upsideDown() { return true; }

  bool useDrawChar() { return true; }

  bool useTilingPatternFill() {
    printf("useTilingPatternFill\n");
    return OutputDev::useTilingPatternFill();
  }

  bool useShadedFills(int type) {
    printf("useShadedFills\n");
    return OutputDev::useShadedFills(type);
  }

  bool useFillColorStop() {
    printf("useFillColorStop\n");
    return OutputDev::useFillColorStop();
  }

  bool useDrawForm() {
    printf("useDrawForm\n");
    return OutputDev::useDrawForm();
  }

  bool interpretType3Chars() {
    printf("interpretType3Chars\n");
    return true;
  }

  bool needNonText() {
    printf("needNonText\n");
    return OutputDev::needNonText();
  }

  bool needCharCount() {
    printf("needCharCount\n");
    return OutputDev::needCharCount();
  }

  bool needClipToCropBox() {
    printf("needClipToCropBox\n");
    return OutputDev::needClipToCropBox();
  }

  void setDefaultCTM(double *ctm) {
    printf("setDefaultCTM\n");
    return OutputDev::setDefaultCTM(ctm);
  }

  bool checkPageSlice(Page *page, double hDPI, double vDPI, int rotate,
                       bool useMediaBox, bool crop, int sliceX, int sliceY,
                       int sliceW, int sliceH, bool printing,
                       bool (*abortCheckCbk)(void *), void *abortCheckCbkData,
                       bool (*annotDisplayDecideCbk)(Annot *, void *),
                       void *annotDisplayDecideCbkData) {
    printf("checkPageSlice\n");
    return OutputDev::checkPageSlice(
        page, hDPI, vDPI, rotate, useMediaBox, crop, sliceX, sliceY, sliceW,
        sliceH, printing, abortCheckCbk, abortCheckCbkData,
        annotDisplayDecideCbk, annotDisplayDecideCbkData);
  }

  void startPage(int pageNum, GfxState *state, XRef *xref) {
    printf("startPage\n");
    return OutputDev::startPage(pageNum, state, xref);
  }

  void endPage() {
    printf("endPage\n");
    return OutputDev::endPage();
  }

  void dump() {
    printf("dump\n");
    return OutputDev::dump();
  }

  void cvtDevToUser(double dx, double dy, double *ux, double *uy) {
    printf("cvtDevToUser\n");
    return OutputDev::cvtDevToUser(dx, dy, ux, uy);
  }

  void cvtUserToDev(double ux, double uy, int *dx, int *dy) {
    printf("cvtUserToDev\n");
    return OutputDev::cvtUserToDev(ux, uy, dx, dy);
  }

  const double *getDefCTM() {
    printf("getDefCTM\n");
    return OutputDev::getDefCTM();
  }

  const double *getDefICTM() {
    printf("getDefICTM\n");
    return OutputDev::getDefICTM();
  }

  void saveState(GfxState *A) {
    printf("saveState\n");
    return OutputDev::saveState(A);
  }

  void restoreState(GfxState *A) {
    printf("restoreState\n");
    return OutputDev::restoreState(A);
  }

  void updateAll(GfxState *state) {
    printf("updateAll\n");
    return OutputDev::updateAll(state);
  }

  void updateCTM(GfxState *A, double B, double C, double D, double E, double F,
                 double G) {
    printf("updateCTM\n");
    return OutputDev::updateCTM(A, B, C, D, E, F, G);
  }

  void updateLineDash(GfxState *A) {
    printf("updateLineDash\n");
    return OutputDev::updateLineDash(A);
  }

  void updateFlatness(GfxState *A) {
    printf("updateFlatness\n");
    return OutputDev::updateFlatness(A);
  }

  void updateLineJoin(GfxState *A) {
    printf("updateLineJoin\n");
    return OutputDev::updateLineJoin(A);
  }

  void updateLineCap(GfxState *A) {
    printf("updateLineCap\n");
    return OutputDev::updateLineCap(A);
  }

  void updateMiterLimit(GfxState *A) {
    printf("updateMiterLimit\n");
    return OutputDev::updateMiterLimit(A);
  }

  void updateLineWidth(GfxState *A) {
    printf("updateLineWidth\n");
    return OutputDev::updateLineWidth(A);
  }

  void updateStrokeAdjust(GfxState *A) {
    printf("updateStrokeAdjust\n");
    return OutputDev::updateStrokeAdjust(A);
  }

  void updateAlphaIsShape(GfxState *A) {
    printf("updateAlphaIsShape\n");
    return OutputDev::updateAlphaIsShape(A);
  }

  void updateTextKnockout(GfxState *A) {
    printf("updateTextKnockout\n");
    return OutputDev::updateTextKnockout(A);
  }

  void updateFillColorSpace(GfxState *A) {
    printf("updateFillColorSpace\n");
    return OutputDev::updateFillColorSpace(A);
  }

  void updateStrokeColorSpace(GfxState *A) {
    printf("updateStrokeColorSpace\n");
    return OutputDev::updateStrokeColorSpace(A);
  }

  void updateFillColor(GfxState *A) {
    printf("updateFillColor\n");
    return OutputDev::updateFillColor(A);
  }

  void updateStrokeColor(GfxState *A) {
    printf("updateStrokeColor\n");
    return OutputDev::updateStrokeColor(A);
  }

  void updateBlendMode(GfxState *A) {
    printf("updateBlendMode\n");
    return OutputDev::updateBlendMode(A);
  }

  void updateFillOpacity(GfxState *A) {
    printf("updateFillOpacity\n");
    return OutputDev::updateFillOpacity(A);
  }

  void updateStrokeOpacity(GfxState *A) {
    printf("updateStrokeOpacity\n");
    return OutputDev::updateStrokeOpacity(A);
  }

  //
  // void updatePatternOpacity(GfxState * A) {
  //   printf("updatePatternOpacity\n");
  //   return OutputDev::updatePatternOpacity(A);
  // }
  //
  //
  // void clearPatternOpacity(GfxState * A) {
  //   printf("clearPatternOpacity\n");
  //   return OutputDev::clearPatternOpacity(A);
  // }

  void updateFillOverprint(GfxState *A) {
    printf("updateFillOverprint\n");
    return OutputDev::updateFillOverprint(A);
  }

  void updateStrokeOverprint(GfxState *A) {
    printf("updateStrokeOverprint\n");
    return OutputDev::updateStrokeOverprint(A);
  }

  void updateOverprintMode(GfxState *A) {
    printf("updateOverprintMode\n");
    return OutputDev::updateOverprintMode(A);
  }

  void updateTransfer(GfxState *A) {
    printf("updateTransfer\n");
    return OutputDev::updateTransfer(A);
  }

  void updateFillColorStop(GfxState *A, double B) {
    printf("updateFillColorStop\n");
    return OutputDev::updateFillColorStop(A, B);
  }

  void updateFont(GfxState *A) {
    printf("updateFont\n");
    return OutputDev::updateFont(A);
  }

  void updateTextMat(GfxState *A) {
    printf("updateTextMat\n");
    return OutputDev::updateTextMat(A);
  }

  void updateCharSpace(GfxState *A) {
    printf("updateCharSpace\n");
    return OutputDev::updateCharSpace(A);
  }

  void updateRender(GfxState *A) {
    printf("updateRender\n");
    return OutputDev::updateRender(A);
  }

  void updateRise(GfxState *A) {
    printf("updateRise\n");
    return OutputDev::updateRise(A);
  }

  void updateWordSpace(GfxState *A) {
    printf("updateWordSpace\n");
    return OutputDev::updateWordSpace(A);
  }

  void updateHorizScaling(GfxState *A) {
    printf("updateHorizScaling\n");
    return OutputDev::updateHorizScaling(A);
  }

  void updateTextPos(GfxState *A) {
    printf("updateTextPos\n");
    return OutputDev::updateTextPos(A);
  }

  void updateTextShift(GfxState *A, double B) {
    printf("updateTextShift\n");
    return OutputDev::updateTextShift(A, B);
  }

  void saveTextPos(GfxState *A) {
    printf("saveTextPos\n");
    return OutputDev::saveTextPos(A);
  }

  void restoreTextPos(GfxState *A) {
    printf("restoreTextPos\n");
    return OutputDev::restoreTextPos(A);
  }

  void stroke(GfxState *A) {
    printf("stroke\n");
    return OutputDev::stroke(A);
  }

  void fill(GfxState *A) {
    printf("fill\n");
    return OutputDev::fill(A);
  }

  void eoFill(GfxState *A) {
    printf("eoFill\n");
    return OutputDev::eoFill(A);
  }

  bool tilingPatternFill(GfxState *A, Gfx *B, Catalog *C, GfxTilingPattern *D, const std::array<double, 6> &E,
                          int F, int G, int H, int I, double J, double K) {

    printf("tilingPatternFill\n");
    return OutputDev::tilingPatternFill(A, B, C, D, E, F, G, H, I, J, K);
  }

  bool functionShadedFill(GfxState *A, GfxFunctionShading *B) {
    printf("functionShadedFill\n");
    return OutputDev::functionShadedFill(A, B);
  }

  bool axialShadedFill(GfxState *A, GfxAxialShading *B, double C, double D) {
    printf("axialShadedFill\n");
    return OutputDev::axialShadedFill(A, B, C, D);
  }

  bool axialShadedSupportExtend(GfxState *A, GfxAxialShading *B) {
    printf("axialShadedSupportExtend\n");
    return OutputDev::axialShadedSupportExtend(A, B);
  }

  bool radialShadedFill(GfxState *A, GfxRadialShading *B, double C, double D) {
    printf("radialShadedFill\n");
    return OutputDev::radialShadedFill(A, B, C, D);
  }

  bool radialShadedSupportExtend(GfxState *A, GfxRadialShading *B) {
    printf("radialShadedSupportExtend\n");
    return OutputDev::radialShadedSupportExtend(A, B);
  }

  bool gouraudTriangleShadedFill(GfxState *state,
                                  GfxGouraudTriangleShading *shading) {
    printf("gouraudTriangleShadedFill\n");
    return OutputDev::gouraudTriangleShadedFill(state, shading);
  }

  bool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading) {
    printf("patchMeshShadedFill\n");
    return OutputDev::patchMeshShadedFill(state, shading);
  }

  void clip(GfxState *A) {
    printf("clip\n");
    return OutputDev::clip(A);
  }

  void eoClip(GfxState *A) {
    printf("eoClip\n");
    return OutputDev::eoClip(A);
  }

  void clipToStrokePath(GfxState *A) {
    printf("clipToStrokePath\n");
    return OutputDev::clipToStrokePath(A);
  }

  void beginStringOp(GfxState *A) {
    printf("beginStringOp\n");
    return OutputDev::beginStringOp(A);
  }

  void endStringOp(GfxState *A) {
    printf("endStringOp\n");
    return OutputDev::endStringOp(A);
  }

  void beginString(GfxState *A, GooString *B) {
    printf("beginString\n");
    return OutputDev::beginString(A, B);
  }

  void endString(GfxState *A) {
    printf("endString\n");
    return OutputDev::endString(A);
  }

  void drawChar(GfxState *A, double B, double C, double D, double E, double F,
                double G, CharCode H, int I, Unicode *J, int K) {
    auto s = toUTF8(J, K);
    // printf("drawChar(%s)\n", char(int(*J)));
    std::cout << "drawChar(" << s << ")" << std::endl;
    return OutputDev::drawChar(A, B, C, D, E, F, G, H, I, J, K);
  }

  void drawString(GfxState *A, GooString *B) {
    printf("drawString\n");
    return OutputDev::drawString(A, B);
  }

  bool beginType3Char(GfxState *A, double B, double C, double D, double E,
                       CharCode F, Unicode *G, int H) {
    printf("beginType3Char\n");
    return OutputDev::beginType3Char(A, B, C, D, E, F, G, H);
  }

  void endType3Char(GfxState *A) {
    printf("endType3Char\n");
    return OutputDev::endType3Char(A);
  }

  void beginTextObject(GfxState *A) {
    printf("beginTextObject\n");
    return OutputDev::beginTextObject(A);
  }

  void endTextObject(GfxState *A) {
    printf("endTextObject\n");
    return OutputDev::endTextObject(A);
  }

  void incCharCount(int A) {
    printf("incCharCount\n");
    return OutputDev::incCharCount(A);
  }

  void beginActualText(GfxState *A, GooString *B) {
    printf("beginActualText\n");
    return OutputDev::beginActualText(A, B);
  }

  void endActualText(GfxState *A) {
    printf("endActualText\n");
    return OutputDev::endActualText(A);
  }

  void drawImageMask(GfxState *state, Object *ref, Stream *str, int width,
                     int height, bool invert, bool interpolate,
                     bool inlineImg) {
    printf("drawImageMask\n");
    return OutputDev::drawImageMask(state, ref, str, width, height, invert,
                                    interpolate, inlineImg);
  }

  void setSoftMaskFromImageMask(GfxState *state, Object *ref, Stream *str,
                                int width, int height, bool invert,
                                bool inlineImg, double *baseMatrix) {
    printf("setSoftMaskFromImageMask\n");
    return OutputDev::setSoftMaskFromImageMask(state, ref, str, width, height,
                                               invert, inlineImg, baseMatrix);
  }

  void unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix) {
    printf("unsetSoftMaskFromImageMask\n");
    return OutputDev::unsetSoftMaskFromImageMask(state, baseMatrix);
  }

  void drawImage(GfxState *state, Object *ref, Stream *str, int width,
                 int height, GfxImageColorMap *colorMap, bool interpolate,
                 int *maskColors, bool inlineImg) {
    printf("drawImage\n");
    return OutputDev::drawImage(state, ref, str, width, height, colorMap,
                                interpolate, maskColors, inlineImg);
  }

  void drawMaskedImage(GfxState *state, Object *ref, Stream *str, int width,
                       int height, GfxImageColorMap *colorMap,
                       bool interpolate, Stream *maskStr, int maskWidth,
                       int maskHeight, bool maskInvert,
                       bool maskInterpolate) {
    printf("drawMaskedImage\n");
    return OutputDev::drawMaskedImage(state, ref, str, width, height, colorMap,
                                      interpolate, maskStr, maskWidth,
                                      maskHeight, maskInvert, maskInterpolate);
  }

  void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str, int width,
                           int height, GfxImageColorMap *colorMap,
                           bool interpolate, Stream *maskStr, int maskWidth,
                           int maskHeight, GfxImageColorMap *maskColorMap,
                           bool maskInterpolate) {
    printf("drawSoftMaskedImage\n");
    return OutputDev::drawSoftMaskedImage(
        state, ref, str, width, height, colorMap, interpolate, maskStr,
        maskWidth, maskHeight, maskColorMap, maskInterpolate);
  }

  void endMarkedContent(GfxState *state) {
    printf("endMarkedContent\n");
    return OutputDev::endMarkedContent(state);
  }

  void beginMarkedContent(char *name, Dict *properties) {
    printf("beginMarkedContent\n");
    return OutputDev::beginMarkedContent(name, properties);
  }

  void markPoint(char *name) {
    printf("markPoint\n");
    return OutputDev::markPoint(name);
  }

  void markPoint(char *name, Dict *properties) {
    printf("markPoint\n");
    return OutputDev::markPoint(name, properties);
  }

  void opiBegin(GfxState *state, Dict *opiDict) {
    printf("opiBegin\n");
    return OutputDev::opiBegin(state, opiDict);
  }

  void opiEnd(GfxState *state, Dict *opiDict) {
    printf("opiEnd\n");
    return OutputDev::opiEnd(state, opiDict);
  }

  void type3D0(GfxState *A, double B, double C) {
    printf("type3D0\n");
    return OutputDev::type3D0(A, B, C);
  }

  void type3D1(GfxState *A, double B, double C, double D, double E, double F,
               double G) {
    printf("type3D1\n");
    return OutputDev::type3D1(A, B, C, D, E, F, G);
  }

  void drawForm(Ref A) {
    printf("drawForm\n");
    return OutputDev::drawForm(A);
  }

  void psXObject(Stream *A, Stream *B) {
    printf("psXObject\n");
    return OutputDev::psXObject(A, B);
  }

  bool checkTransparencyGroup(GfxState *A, bool B) {
    printf("checkTransparencyGroup\n");
    return OutputDev::checkTransparencyGroup(A, B);
  }

  void beginTransparencyGroup(GfxState *A, const std::array<double, 4> &B, GfxColorSpace *C, bool D,
                              bool E, bool F) {
    printf("beginTransparencyGroup\n");
    return OutputDev::beginTransparencyGroup(A, B, C, D, E, F);
  }

  void endTransparencyGroup(GfxState *A) {
    printf("endTransparencyGroup\n");
    return OutputDev::endTransparencyGroup(A);
  }

  void paintTransparencyGroup(GfxState *A, const std::array<double, 4> &B) {
    printf("paintTransparencyGroup\n");
    return OutputDev::paintTransparencyGroup(A, B);
  }

  void setSoftMask(GfxState *A, const std::array<double, 4> &B, bool C, Function *D, GfxColor *E) {
    printf("setSoftMask\n");
    return OutputDev::setSoftMask(A, B, C, D, E);
  }

  void clearSoftMask(GfxState *A) {
    printf("clearSoftMask\n");
    return OutputDev::clearSoftMask(A);
  }

  void processLink(AnnotLink *A) {
    printf("processLink\n");
    return OutputDev::processLink(A);
  }

  bool getVectorAntialias() {
    printf("getVectorAntialias\n");
    return OutputDev::getVectorAntialias();
  }

  void setVectorAntialias(bool A) {
    printf("setVectorAntialias\n");
    return OutputDev::setVectorAntialias(A);
  }
};
