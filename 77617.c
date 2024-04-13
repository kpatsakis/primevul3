void GLES2DecoderImpl::DoCopyTexImage2D(
  GLenum target,
  GLint level,
  GLenum internal_format,
  GLint x,
  GLint y,
  GLsizei width,
  GLsizei height,
  GLint border) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glCopyTexImage2D: unknown texture for target");
    return;
  }
  if (info->IsImmutable()) {
    SetGLError(GL_INVALID_OPERATION,
               "glCopyTexImage2D: texture is immutable");
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, 1) ||
      border != 0) {
    SetGLError(GL_INVALID_VALUE, "glCopyTexImage2D: dimensions out of range");
    return;
  }

  GLenum read_format = GetBoundReadFrameBufferInternalFormat();
  uint32 channels_exist = GLES2Util::GetChannelsForFormat(read_format);
  uint32 channels_needed = GLES2Util::GetChannelsForFormat(internal_format);

  if ((channels_needed & channels_exist) != channels_needed) {
    SetGLError(GL_INVALID_OPERATION, "glCopyTexImage2D: incompatible format");
    return;
  }

  if (!CheckBoundFramebuffersValid("glCopyTexImage2D")) {
    return;
  }

  CopyRealGLErrorsToWrapper();
  ScopedResolvedFrameBufferBinder binder(this, false, true);
  gfx::Size size = GetBoundReadFrameBufferSize();

  if (info->IsAttachedToFramebuffer()) {
    state_dirty_ = true;
    framebuffer_manager()->IncFramebufferStateChangeCount();
  }

  GLint copyX = 0;
  GLint copyY = 0;
  GLint copyWidth = 0;
  GLint copyHeight = 0;
  Clip(x, width, size.width(), &copyX, &copyWidth);
  Clip(y, height, size.height(), &copyY, &copyHeight);

  if (copyX != x ||
      copyY != y ||
      copyWidth != width ||
      copyHeight != height) {
    if (!ClearLevel(
        info->service_id(), info->target(),
        target, level, internal_format, GL_UNSIGNED_BYTE, width, height,
        info->IsImmutable())) {
      SetGLError(GL_OUT_OF_MEMORY, "glCopyTexImage2D: dimensions too big");
      return;
    }
    if (copyHeight > 0 && copyWidth > 0) {
      GLint dx = copyX - x;
      GLint dy = copyY - y;
      GLint destX = dx;
      GLint destY = dy;
      glCopyTexSubImage2D(target, level,
                          destX, destY, copyX, copyY,
                          copyWidth, copyHeight);
    }
  } else {
    glCopyTexImage2D(target, level, internal_format,
                     copyX, copyY, copyWidth, copyHeight, border);
  }
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(
        info, target, level, internal_format, width, height, 1,
        border, internal_format, GL_UNSIGNED_BYTE, true);
  }
}
