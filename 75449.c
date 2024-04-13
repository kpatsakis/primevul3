void ConvertYUVToRGB32(const uint8* y_buf,
                       const uint8* u_buf,
                       const uint8* v_buf,
                       uint8* rgb_buf,
                       int width,
                       int height,
                       int y_pitch,
                       int uv_pitch,
                       int rgb_pitch,
                       YUVType yuv_type) {
  unsigned int y_shift = yuv_type;
  for (int y = 0; y < height; ++y) {
    uint8* rgb_row = rgb_buf + y * rgb_pitch;
    const uint8* y_ptr = y_buf + y * y_pitch;
    const uint8* u_ptr = u_buf + (y >> y_shift) * uv_pitch;
    const uint8* v_ptr = v_buf + (y >> y_shift) * uv_pitch;

    FastConvertYUVToRGB32Row(y_ptr,
                             u_ptr,
                             v_ptr,
                             rgb_row,
                             width);
  }

  EMMS();
}
