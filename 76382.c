void GLES2DecoderImpl::DoFramebufferTexture2D(
    GLenum target, GLenum attachment, GLenum textarget,
    GLuint client_texture_id, GLint level) {
  if (!bound_framebuffer_) {
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
  glFramebufferTexture2DEXT(target, attachment, textarget, service_id, level);
  if (service_id != 0 &&
      glCheckFramebufferStatusEXT(target) == GL_FRAMEBUFFER_COMPLETE) {
    ClearUnclearedRenderbuffers(bound_framebuffer_);
  }
}
