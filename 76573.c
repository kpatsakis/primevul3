void ChromotingInstance::SetCursorShape(
    const protocol::CursorShapeInfo& cursor_shape) {
  if (!cursor_shape.has_data() ||
      !cursor_shape.has_width() ||
      !cursor_shape.has_height() ||
      !cursor_shape.has_hotspot_x() ||
      !cursor_shape.has_hotspot_y()) {
    return;
  }

  int width = cursor_shape.width();
  int height = cursor_shape.height();

  if (width < 0 || height < 0) {
    return;
  }

  if (width > 32 || height > 32) {
    VLOG(2) << "Cursor too large for SetCursor: "
            << width << "x" << height << " > 32x32";
    return;
  }

  if (pp::ImageData::GetNativeImageDataFormat() !=
      PP_IMAGEDATAFORMAT_BGRA_PREMUL) {
    VLOG(2) << "Unable to set cursor shape - non-native image format";
    return;
  }

  int hotspot_x = cursor_shape.hotspot_x();
  int hotspot_y = cursor_shape.hotspot_y();

  pp::ImageData cursor_image(this, PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                             pp::Size(width, height), false);

  int bytes_per_row = width * kBytesPerPixel;
  const uint8* src_row_data = reinterpret_cast<const uint8*>(
      cursor_shape.data().data());
  uint8* dst_row_data = reinterpret_cast<uint8*>(cursor_image.data());
  for (int row = 0; row < height; row++) {
    memcpy(dst_row_data, src_row_data, bytes_per_row);
    src_row_data += bytes_per_row;
    dst_row_data += cursor_image.stride();
  }

  pp::MouseCursor::SetCursor(this, PP_MOUSECURSOR_TYPE_CUSTOM,
                             cursor_image,
                             pp::Point(hotspot_x, hotspot_y));
}
