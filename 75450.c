void ConvertYUY2ToYUV(const uint8* src,
                      uint8* yplane,
                      uint8* uplane,
                      uint8* vplane,
                      int width,
                      int height) {
  ConvertYUY2ToYUV_C(src, yplane, uplane, vplane, width, height);
}
