void GLES2DecoderImpl::DoTexSubImage2D(
  GLenum target,
  GLint level,
  GLint xoffset,
  GLint yoffset,
  GLsizei width,
  GLsizei height,
  GLenum format,
  GLenum type,
  const void * data) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexSubImage2D: unknown texture for target");
    return;
  }
  if (!info->ValidForTexture(
          target, level, xoffset, yoffset, width, height, format, type)) {
    SetGLError(GL_INVALID_VALUE,
               "glTexSubImage2D: bad dimensions.");
    return;
  }
  glTexSubImage2D(
      target, level, xoffset, yoffset, width, height, format, type, data);
}
