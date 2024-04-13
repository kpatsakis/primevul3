error::Error GLES2DecoderImpl::DoCompressedTexImage2D(
  GLenum target,
  GLint level,
  GLenum internal_format,
  GLsizei width,
  GLsizei height,
  GLint border,
  GLsizei image_size,
  const void* data) {
  if (!validators_->texture_target.IsValid(target)) {
    SetGLError(GL_INVALID_ENUM,
               "glCompressedTexImage2D: target GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->compressed_texture_format.IsValid(
      internal_format)) {
    SetGLError(GL_INVALID_ENUM,
               "glCompressedTexImage2D: internal_format GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, 1) ||
      border != 0) {
    SetGLError(GL_INVALID_VALUE,
               "glCompressedTexImage2D: dimensions out of range");
    return error::kNoError;
  }
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE,
               "glCompressedTexImage2D: unknown texture target");
    return error::kNoError;
  }
  if (info->IsImmutable()) {
    SetGLError(GL_INVALID_OPERATION,
               "glCompressedTexImage2D: texture is immutable");
    return error::kNoError;
  }

  if (info->IsAttachedToFramebuffer()) {
    state_dirty_ = true;
    framebuffer_manager()->IncFramebufferStateChangeCount();
  }

  scoped_array<int8> zero;
  if (!data) {
    zero.reset(new int8[image_size]);
    memset(zero.get(), 0, image_size);
    data = zero.get();
  }
  CopyRealGLErrorsToWrapper();
  glCompressedTexImage2D(
      target, level, internal_format, width, height, border, image_size, data);
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(
        info, target, level, internal_format, width, height, 1, border, 0, 0,
        true);
  }
  return error::kNoError;
}
