void GLES2DecoderImpl::Destroy() {
#if defined(GLES2_GPU_SERVICE_TRANSLATE_SHADER)
  if (vertex_compiler_ != NULL) {
    ShDestruct(vertex_compiler_);
    vertex_compiler_ = NULL;
  }
  if (fragment_compiler_ != NULL) {
    ShDestruct(fragment_compiler_);
    fragment_compiler_ = NULL;
  }
#endif  // GLES2_GPU_SERVICE_TRANSLATE_SHADER)

  if (context_.get()) {
    MakeCurrent();

    if (black_2d_texture_id_) {
      glDeleteTextures(1, &black_2d_texture_id_);
    }
    if (black_cube_texture_id_) {
      glDeleteTextures(1, &black_cube_texture_id_);
    }
    if (attrib_0_buffer_id_) {
      glDeleteBuffersARB(1, &attrib_0_buffer_id_);
    }

    if (parent_) {
      GLuint service_id = offscreen_saved_color_texture_->id();
      GLuint client_id = 0;
      if (parent_->texture_manager()->GetClientId(service_id, &client_id)) {
        parent_->texture_manager()->RemoveTextureInfo(client_id);
      }
    }

    if (offscreen_target_frame_buffer_.get()) {
      offscreen_target_frame_buffer_->Destroy();
      offscreen_target_frame_buffer_.reset();
    }

    if (offscreen_target_color_texture_.get()) {
      offscreen_target_color_texture_->Destroy();
      offscreen_target_color_texture_.reset();
    }

    if (offscreen_target_depth_render_buffer_.get()) {
      offscreen_target_depth_render_buffer_->Destroy();
      offscreen_target_depth_render_buffer_.reset();
    }

    if (offscreen_target_stencil_render_buffer_.get()) {
      offscreen_target_stencil_render_buffer_->Destroy();
      offscreen_target_stencil_render_buffer_.reset();
    }

    if (offscreen_saved_color_texture_.get()) {
      offscreen_saved_color_texture_->Destroy();
      offscreen_saved_color_texture_.reset();
    }

    context_->Destroy();
    context_.reset();
  }

  if (default_context_.get()) {
    default_context_->Destroy();
    default_context_.reset();
  }
}
