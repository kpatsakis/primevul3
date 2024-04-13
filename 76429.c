error::Error GLES2DecoderImpl::HandleSwapBuffers(
    uint32 immediate_data_size, const gles2::SwapBuffers& c) {
  if (offscreen_target_frame_buffer_.get()) {
    ScopedGLErrorSuppressor suppressor(this);

    if (!UpdateOffscreenFrameBufferSize())
      return error::kLostContext;

    if (parent_) {
      ScopedFrameBufferBinder binder(this,
                                     offscreen_target_frame_buffer_->id());
      offscreen_saved_color_texture_->Copy(
          offscreen_saved_color_texture_->size());
    }
  } else {
    context_->SwapBuffers();
  }


  if (swap_buffers_callback_.get()) {
    swap_buffers_callback_->Run();
  }

  return error::kNoError;
}
