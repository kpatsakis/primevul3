ScopedFrameBufferBinder::~ScopedFrameBufferBinder() {
  ScopedGLErrorSuppressor suppressor(decoder_);
  FramebufferManager::FramebufferInfo* info =
      decoder_->bound_framebuffer_.get();
  GLuint framebuffer_id = info ? info->service_id() : 0;
  if (framebuffer_id == 0 &&
      decoder_->offscreen_target_frame_buffer_.get()) {
    glBindFramebufferEXT(GL_FRAMEBUFFER,
                         decoder_->offscreen_target_frame_buffer_->id());
  } else {
    glBindFramebufferEXT(GL_FRAMEBUFFER, framebuffer_id);
  }
}
