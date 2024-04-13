GLenum GLES2DecoderImpl::DoCheckFramebufferStatus(GLenum target) {
  if (!bound_framebuffer_) {
    return GL_FRAMEBUFFER_COMPLETE;
  }
  return glCheckFramebufferStatusEXT(target);
}
