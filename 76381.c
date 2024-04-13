void GLES2DecoderImpl::DoFramebufferRenderbuffer(
    GLenum target, GLenum attachment, GLenum renderbuffertarget,
    GLuint client_renderbuffer_id) {
  if (!bound_framebuffer_) {
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
  glFramebufferRenderbufferEXT(
      target, attachment, renderbuffertarget, service_id);
  if (service_id == 0 ||
      glCheckFramebufferStatusEXT(target) == GL_FRAMEBUFFER_COMPLETE) {
    bound_framebuffer_->AttachRenderbuffer(attachment, info);
    if (info) {
      ClearUnclearedRenderbuffers(bound_framebuffer_);
    }
  }
}
