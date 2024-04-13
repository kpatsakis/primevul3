bool GLES2DecoderImpl::ClearLevel(
    unsigned service_id,
    unsigned bind_target,
    unsigned target,
    int level,
    unsigned format,
    unsigned type,
    int width,
    int height,
    bool is_texture_immutable) {
  uint32 pixels_size = 0;
  if (!GLES2Util::ComputeImageDataSize(
      width, height, format, type, unpack_alignment_, &pixels_size)) {
    return false;
  }
  scoped_array<char> zero(new char[pixels_size]);
  memset(zero.get(), 0, pixels_size);
  glBindTexture(bind_target, service_id);
  if (is_texture_immutable) {
    glTexSubImage2D(
        target, level, 0, 0, width, height, format, type, zero.get());
  } else {
    WrappedTexImage2D(
        target, level, format, width, height, 0, format, type, zero.get());
  }
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(bind_target);
  glBindTexture(bind_target, info ? info->service_id() : 0);
  return true;
}
