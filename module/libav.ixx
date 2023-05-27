module;

#ifdef __MINGW64__
#  include <cwchar> // work around ODR problems with the C standard library
#endif

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __clang__
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
}
#endif

export module libav;

#if _MSC_VER
#   pragma comment(lib, "libav.lib")
#endif

#ifndef __clang__
export {
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
}
}
#else

export using ::AVCodecContext;
export using ::avcodec_alloc_context3;
export using ::avcodec_free_context;
export using ::AVFormatContext;
export using ::avformat_open_input;
export using ::avformat_close_input;
export using ::AVFrame;
export using ::av_frame_alloc;
export using ::av_frame_free;
export using ::av_frame_unref;
export using ::AVPacket;
export using ::av_packet_alloc;
export using ::av_packet_free;
export using ::av_packet_unref;
export using ::AVCodec;
export using ::av_find_best_stream;
export using ::avformat_find_stream_info;
export using ::avcodec_parameters_to_context;
export using ::avcodec_open2;
export using ::av_rescale_q;
export using ::av_read_frame;
export using ::avcodec_receive_frame;
export using ::avcodec_send_packet;
export using ::AVPixelFormat;

#endif

#undef EOF

#define DCL(x) constexpr inline auto x = AV_##x
#define EXP(x) export constexpr inline auto AV_##x = libav::x
#define DCLERR(x) constexpr inline auto x = AVERROR_##x
#define EXPERR(x) export constexpr inline auto AVERROR_##x = libav::error::x

// this is the bare minimum!
// to export all libav macro definitions use X-macros

export namespace libav {
DCL(TIME_BASE);

namespace error {
DCLERR(EOF);
} // namespace error
} // namespace libav

#undef AV_TIME_BASE
#undef AVERROR_EOF

EXP(TIME_BASE);
EXPERR(EOF);
