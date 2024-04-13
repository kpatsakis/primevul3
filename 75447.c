void ConvertRGB24ToYUV(const uint8* rgbframe,
                       uint8* yplane,
                       uint8* uplane,
                       uint8* vplane,
                       int width,
                       int height,
                       int rgbstride,
                       int ystride,
                       int uvstride) {
  ConvertRGB24ToYUV_C(rgbframe, yplane, uplane, vplane, width, height,
                      rgbstride, ystride, uvstride);
}
