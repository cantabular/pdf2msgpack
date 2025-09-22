#! /usr/bin/env python3
top = '.'
out = 'build'

from waflib.TaskGen import after_method, feature


@feature('static_linking')
@after_method('apply_link', 'propagate_uselib_vars')
def force_static_linking(self):
    env = self.link_task.env
    env.STLIB += env.LIB
    env.LIB = []
    env.STLIB_MARKER = '-static'
    env.SHLIB_MARKER = ''


def options(opt):
    opt.load('compiler_cxx')

    opt.add_option('--static', action='store_true', default=False,
                   help='Build static binaries')

    opt.add_option('--sanitize', action='store', default=False,
                   help='Use -fsanitize={leak,address,undefined}')

    opt.add_option('--release', action='store_true', default=False,
                   help='Enable optimizations')

    opt.add_option('--enable-syscall-reporter', action='store_true',
                   default=False, help='Enable syscall-reporter')

    opt.add_option('--disable-syscall-filter', action='store_true',
                   default=False, help='Disable syscall filter')


def configure(ctx):
    ctx.load('compiler_cxx')

    ctx.check(features='cxx cxxprogram', cxxflags="--std=c++20")

    ctx.env.append_value("CXXFLAGS", [
        "-g",
        "-Wall",
        "-Werror",
        "-ansi",
        "--std=c++20",
        "-DMSGPACK_NO_BOOST",
    ])

    need_syscall_filter = not ctx.options.disable_syscall_filter
    ctx.check_cxx(header_name="linux/seccomp.h",
                  msg="Checking linux-headers & seccomp support",
                  mandatory=need_syscall_filter)

    if ctx.options.release:
        ctx.env.append_value("CXXFLAGS", ["-O2"])

    if ctx.options.sanitize:
        ctx.msg("Enable sanitizer", ctx.options.sanitize)
        param = "-fsanitize={}".format(ctx.options.sanitize)
        ctx.env.append_value("CXXFLAGS", [param])
        ctx.env.append_value("LINKFLAGS", [param])

    if ctx.options.static:
        ctx.msg("Building static binaries", "yes")

        ctx.env.BUILD_STATIC = True

        ctx.check_cfg(package='poppler', uselib_store='poppler',
                      args=['--cflags', '--libs', '--static'])

        poppler_stlib = (
            "fontconfig uuid intl expat freetype lcms2 openjp2 jpeg png bz2 z pthread"
        )
        for lib in poppler_stlib.split():
            ctx.check_cxx(stlib=lib, uselib_store='poppler')

    else:
        ctx.msg("Building dynamic binaries", "yes")

        ctx.check_cfg(package='poppler', uselib_store='poppler',
                      args=['--cflags', '--libs'])

    # /usr/include/poppler/...
    ctx.check_cxx(header_name="PDFDoc.h", use="poppler",
                  msg="Checking libpoppler-private-dev (poppler configured " +
                      "with ENABLE_UNSTABLE_API_ABI_HEADERS=ON)")

    if ctx.options.enable_syscall_reporter:
        ctx.env.append_value("CXXFLAGS", ["-DENABLE_SYSCALL_REPORTER"])
        ctx.msg("Enable syscall reporter (***NOT FOR PRODUCTION***)", "yes",
                color="RED")

    ctx.env.DISABLE_SYSCALL_FILTER = ctx.options.disable_syscall_filter
    if ctx.env.DISABLE_SYSCALL_FILTER:
        ctx.env.append_value("CXXFLAGS", ["-DDISABLE_SYSCALL_FILTER"])
        ctx.msg("Disable syscall filter (***NOT FOR PRODUCTION***)", "yes",
                color="RED")


def build(ctx):
    features = ""

    if ctx.env.BUILD_STATIC:
        features = 'static_linking'

    sources = ctx.path.ant_glob('src/main.cpp')
    if not ctx.env.DISABLE_SYSCALL_FILTER:
        sources += ctx.path.ant_glob('src/syscall-reporter.cpp')

    ctx.program(
        source=sources,
        target="pdf2msgpack",
        use="poppler",
        features=features,
        includes=["vendor/github.com/msgpack/msgpack-c/include"],
    )
