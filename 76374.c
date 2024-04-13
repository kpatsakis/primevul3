void GLES2DecoderImpl::DoCompressedTexSubImage2D(
  GLenum target,
  GLint level,
  GLint xoffset,
  GLint yoffset,
  GLsizei width,
  GLsizei height,
  GLenum format,
  GLsizei image_size,
  const void * data) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glCompressedTexSubImage2D: unknown texture for target");
    return;
  }
  GLenum type = 0;
  GLenum dummy = 0;
  if (!info->GetLevelType(target, level, &type, &dummy) ||
      !info->ValidForTexture(
          target, level, xoffset, yoffset, width, height, format, type)) {
    SetGLError(GL_INVALID_VALUE,
               "glCompressdTexSubImage2D: bad dimensions.");
    return;
  }
  glCompressedTexSubImage2D(
      target, level, xoffset, yoffset, width, height, format, image_size, data);
}
