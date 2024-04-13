void GLES2DecoderImpl::DoCopyTexSubImage2D(
  GLenum target,
  GLint level,
  GLint xoffset,
  GLint yoffset,
  GLint x,
  GLint y,
  GLsizei width,
  GLsizei height) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glCopyTexSubImage2D: unknown texture for target");
    return;
  }
  GLenum type = 0;
  GLenum format = 0;
  if (!info->GetLevelType(target, level, &type, &format) ||
      !info->ValidForTexture(
          target, level, xoffset, yoffset, width, height, format, type)) {
    SetGLError(GL_INVALID_VALUE,
               "glCopyTexSubImage2D: bad dimensions.");
    return;
  }
  glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}
