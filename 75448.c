void ConvertRGB32ToYUV(const uint8* rgbframe,
                       uint8* yplane,
                       uint8* uplane,
                       uint8* vplane,
                       int width,
                       int height,
                       int rgbstride,
                       int ystride,
                       int uvstride) {
  static void (*convert_proc)(const uint8*, uint8*, uint8*, uint8*,
                              int, int, int, int, int) = NULL;
  if (!convert_proc) {
#if defined(ARCH_CPU_ARM_FAMILY)
    convert_proc = &ConvertRGB32ToYUV_C;
#else
    if (hasSSE2())
      convert_proc = &ConvertRGB32ToYUV_SSE2;
    else
      convert_proc = &ConvertRGB32ToYUV_C;
#endif
  }

  convert_proc(rgbframe, yplane, uplane, vplane, width, height,
               rgbstride, ystride, uvstride);
}
