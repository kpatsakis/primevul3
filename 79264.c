void TabsCaptureVisibleTabFunction::SendResultFromBitmap(
    const SkBitmap& screen_capture) {
  std::vector<unsigned char> data;
  SkAutoLockPixels screen_capture_lock(screen_capture);
  bool encoded = false;
  std::string mime_type;
  switch (image_format_) {
    case FORMAT_JPEG:
      encoded = gfx::JPEGCodec::Encode(
          reinterpret_cast<unsigned char*>(screen_capture.getAddr32(0, 0)),
          gfx::JPEGCodec::FORMAT_SkBitmap,
          screen_capture.width(),
          screen_capture.height(),
          static_cast<int>(screen_capture.rowBytes()),
          image_quality_,
          &data);
      mime_type = keys::kMimeTypeJpeg;
      break;
    case FORMAT_PNG:
      encoded = gfx::PNGCodec::EncodeBGRASkBitmap(
          screen_capture,
          true,  // Discard transparency.
          &data);
      mime_type = keys::kMimeTypePng;
      break;
    default:
      NOTREACHED() << "Invalid image format.";
  }

  if (!encoded) {
    error_ = keys::kInternalVisibleTabCaptureError;
    SendResponse(false);
    return;
  }

  std::string base64_result;
  base::StringPiece stream_as_string(
      reinterpret_cast<const char*>(vector_as_array(&data)), data.size());

  base::Base64Encode(stream_as_string, &base64_result);
  base64_result.insert(0, base::StringPrintf("data:%s;base64,",
                                             mime_type.c_str()));
  SetResult(new StringValue(base64_result));
  SendResponse(true);
}
