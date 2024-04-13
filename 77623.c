void GLES2DecoderImpl::DoFramebufferTexture2D(
    GLenum target, GLenum attachment, GLenum textarget,
    GLuint client_texture_id, GLint level) {
  FramebufferManager::FramebufferInfo* framebuffer_info =
      GetFramebufferInfoForTarget(target);
  if (!framebuffer_info) {
    SetGLError(GL_INVALID_OPERATION,
               "glFramebufferTexture2D: no framebuffer bound.");
    return;
  }
  GLuint service_id = 0;
  TextureManager::TextureInfo* info = NULL;
  if (client_texture_id) {
    info = GetTextureInfo(client_texture_id);
    if (!info) {
      SetGLError(GL_INVALID_OPERATION,
                 "glFramebufferTexture2D: unknown texture");
      return;
    }
    service_id = info->service_id();
  }

  if (!texture_manager()->ValidForTarget(textarget, level, 0, 0, 1)) {
    SetGLError(GL_INVALID_VALUE,
               "glFramebufferTexture2D: level out of range");
    return;
  }

  CopyRealGLErrorsToWrapper();
  glFramebufferTexture2DEXT(target, attachment, textarget, service_id, level);
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    framebuffer_info->AttachTexture(attachment, info, textarget, level);
  }
  if (framebuffer_info == bound_draw_framebuffer_) {
    state_dirty_ = true;
  }
}
