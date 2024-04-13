void GLES2DecoderImpl::DoFramebufferRenderbuffer(
    GLenum target, GLenum attachment, GLenum renderbuffertarget,
    GLuint client_renderbuffer_id) {
  FramebufferManager::FramebufferInfo* framebuffer_info =
      GetFramebufferInfoForTarget(target);
  if (!framebuffer_info) {
    SetGLError(GL_INVALID_OPERATION,
               "glFramebufferRenderbuffer: no framebuffer bound");
    return;
  }
  GLuint service_id = 0;
  RenderbufferManager::RenderbufferInfo* info = NULL;
  if (client_renderbuffer_id) {
    info = GetRenderbufferInfo(client_renderbuffer_id);
    if (!info) {
      SetGLError(GL_INVALID_OPERATION,
                 "glFramebufferRenderbuffer: unknown renderbuffer");
      return;
    }
    service_id = info->service_id();
  }
  CopyRealGLErrorsToWrapper();
  glFramebufferRenderbufferEXT(
      target, attachment, renderbuffertarget, service_id);
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    framebuffer_info->AttachRenderbuffer(attachment, info);
  }
  if (framebuffer_info == bound_draw_framebuffer_) {
    state_dirty_ = true;
  }
}
