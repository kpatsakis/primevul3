GLenum GLES2DecoderImpl::GetBoundReadFrameBufferInternalFormat() {
  FramebufferManager::FramebufferInfo* framebuffer =
      GetFramebufferInfoForTarget(GL_READ_FRAMEBUFFER);
  if (framebuffer != NULL) {
    return framebuffer->GetColorAttachmentFormat();
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_color_format_;
  } else {
    return back_buffer_color_format_;
  }
}
