void GLES2DecoderImpl::DeleteRenderbuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  bool supports_seperate_framebuffer_binds =
     feature_info_->feature_flags().chromium_framebuffer_multisample;
  for (GLsizei ii = 0; ii < n; ++ii) {
    RenderbufferManager::RenderbufferInfo* renderbuffer =
        GetRenderbufferInfo(client_ids[ii]);
    if (renderbuffer && !renderbuffer->IsDeleted()) {
      if (bound_renderbuffer_ == renderbuffer) {
        bound_renderbuffer_ = NULL;
      }
      if (supports_seperate_framebuffer_binds) {
        if (bound_read_framebuffer_) {
          bound_read_framebuffer_->UnbindRenderbuffer(
              GL_READ_FRAMEBUFFER, renderbuffer);
        }
        if (bound_draw_framebuffer_) {
          bound_draw_framebuffer_->UnbindRenderbuffer(
              GL_DRAW_FRAMEBUFFER, renderbuffer);
        }
      } else {
        if (bound_draw_framebuffer_) {
          bound_draw_framebuffer_->UnbindRenderbuffer(
              GL_FRAMEBUFFER, renderbuffer);
        }
      }
      state_dirty_ = true;
      RemoveRenderbufferInfo(client_ids[ii]);
    }
  }
}
