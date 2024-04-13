error::Error GLES2DecoderImpl::HandleResizeCHROMIUM(
    uint32 immediate_data_size, const gles2::ResizeCHROMIUM& c) {
  GLuint width = static_cast<GLuint>(c.width);
  GLuint height = static_cast<GLuint>(c.height);
  TRACE_EVENT2("gpu", "glResizeChromium", "width", width, "height", height);
#if defined(OS_POSIX) && !defined(OS_MACOSX) && \
    !defined(UI_COMPOSITOR_IMAGE_TRANSPORT)
  glFinish();
#endif
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();
  if (is_offscreen) {
    if (!ResizeOffscreenFrameBuffer(gfx::Size(width, height)))
      return error::kLostContext;
  }

  if (!resize_callback_.is_null()) {
    resize_callback_.Run(gfx::Size(width, height));
    DCHECK(context_->IsCurrent(surface_.get()));
    if (!context_->IsCurrent(surface_.get()))
      return error::kLostContext;
  }

  UpdateBackbufferMemoryAccounting();

  return error::kNoError;
}
