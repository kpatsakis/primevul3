void GLES2DecoderImpl::DoTexStorage2DEXT(
  GLenum target,
  GLint levels,
  GLenum internal_format,
  GLsizei width,
  GLsizei height) {
  if (!texture_manager()->ValidForTarget(target, 0, width, height, 1) ||
      TextureManager::ComputeMipMapCount(width, height, 1) < levels) {
    SetGLError(GL_INVALID_VALUE, "glTexStorage2DEXT: dimensions out of range");
    return;
  }
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexStorage2DEXT: unknown texture for target");
    return;
  }
  if (info->IsAttachedToFramebuffer()) {
    state_dirty_ = true;
  }
  if (info->IsImmutable()) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexStorage2DEXT: texture is immutable");
    return;
  }
  CopyRealGLErrorsToWrapper();
  glTexStorage2DEXT(target, levels, GetTexInternalFormat(internal_format),
                    width, height);
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    GLenum format = ExtractFormatFromStorageFormat(internal_format);
    GLenum type = ExtractTypeFromStorageFormat(internal_format);
    GLsizei level_width = width;
    GLsizei level_height = height;
    for (int ii = 0; ii < levels; ++ii) {
      texture_manager()->SetLevelInfo(
          info, target, 0, format, level_width, level_height, 1, 0, format,
          type, false);
      level_width = std::max(1, level_width >> 1);
      level_height = std::max(1, level_height >> 1);
    }
    info->SetImmutable(true);
  }
}
