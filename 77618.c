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

  GLenum read_format = GetBoundReadFrameBufferInternalFormat();
  uint32 channels_exist = GLES2Util::GetChannelsForFormat(read_format);
  uint32 channels_needed = GLES2Util::GetChannelsForFormat(format);

  if ((channels_needed & channels_exist) != channels_needed) {
    SetGLError(
        GL_INVALID_OPERATION, "glCopyTexSubImage2D: incompatible format");
    return;
  }

  if (!CheckBoundFramebuffersValid("glCopyTexSubImage2D")) {
    return;
  }

  ScopedResolvedFrameBufferBinder binder(this, false, true);
  gfx::Size size = GetBoundReadFrameBufferSize();
  GLint copyX = 0;
  GLint copyY = 0;
  GLint copyWidth = 0;
  GLint copyHeight = 0;
  Clip(x, width, size.width(), &copyX, &copyWidth);
  Clip(y, height, size.height(), &copyY, &copyHeight);

  if (!texture_manager()->ClearTextureLevel(this, info, target, level)) {
    SetGLError(GL_OUT_OF_MEMORY, "glCopyTexSubImage2D: dimensions too big");
    return;
  }

  if (copyX != x ||
      copyY != y ||
      copyWidth != width ||
      copyHeight != height) {
    uint32 pixels_size = 0;
    if (!GLES2Util::ComputeImageDataSize(
        width, height, format, type, unpack_alignment_, &pixels_size)) {
      SetGLError(GL_INVALID_VALUE, "glCopyTexSubImage2D: dimensions too large");
      return;
    }
    scoped_array<char> zero(new char[pixels_size]);
    memset(zero.get(), 0, pixels_size);
    glTexSubImage2D(
        target, level, xoffset, yoffset, width, height,
        format, type, zero.get());
  }

  if (copyHeight > 0 && copyWidth > 0) {
    GLint dx = copyX - x;
    GLint dy = copyY - y;
    GLint destX = xoffset + dx;
    GLint destY = yoffset + dy;
    glCopyTexSubImage2D(target, level,
                        destX, destY, copyX, copyY,
                        copyWidth, copyHeight);
  }
}
