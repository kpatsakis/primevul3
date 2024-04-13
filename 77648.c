gfx::Size GLES2DecoderImpl::GetBoundReadFrameBufferSize() {
  FramebufferManager::FramebufferInfo* framebuffer =
      GetFramebufferInfoForTarget(GL_READ_FRAMEBUFFER);
  if (framebuffer != NULL) {
    const FramebufferManager::FramebufferInfo::Attachment* attachment =
        framebuffer->GetAttachment(GL_COLOR_ATTACHMENT0);
    if (attachment) {
      return gfx::Size(attachment->width(), attachment->height());
    }
    return gfx::Size(0, 0);
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_size_;
  } else {
    return surface_->GetSize();
  }
}
