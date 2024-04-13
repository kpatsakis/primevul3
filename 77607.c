void GLES2DecoderImpl::Destroy() {
  bool have_context = context_.get() && MakeCurrent();

  ChildList children = children_;
  for (ChildList::iterator it = children.begin(); it != children.end(); ++it)
    (*it)->SetParent(NULL, 0);
  DCHECK(children_.empty());
  SetParent(NULL, 0);

  vertex_attrib_manager_.reset();
  texture_units_.reset();
  bound_array_buffer_ = NULL;
  bound_element_array_buffer_ = NULL;
  current_query_ = NULL;
  current_program_ = NULL;
  bound_read_framebuffer_ = NULL;
  bound_draw_framebuffer_ = NULL;
  bound_renderbuffer_ = NULL;

  if (have_context) {
    if (current_program_) {
      program_manager()->UnuseProgram(shader_manager(), current_program_);
      current_program_ = NULL;
    }

    if (attrib_0_buffer_id_) {
      glDeleteBuffersARB(1, &attrib_0_buffer_id_);
    }
    if (fixed_attrib_buffer_id_) {
      glDeleteBuffersARB(1, &fixed_attrib_buffer_id_);
    }

    if (offscreen_target_frame_buffer_.get())
      offscreen_target_frame_buffer_->Destroy();
    if (offscreen_target_color_texture_.get())
      offscreen_target_color_texture_->Destroy();
    if (offscreen_target_color_render_buffer_.get())
      offscreen_target_color_render_buffer_->Destroy();
    if (offscreen_target_depth_render_buffer_.get())
      offscreen_target_depth_render_buffer_->Destroy();
    if (offscreen_target_stencil_render_buffer_.get())
      offscreen_target_stencil_render_buffer_->Destroy();
    if (offscreen_saved_frame_buffer_.get())
      offscreen_saved_frame_buffer_->Destroy();
    if (offscreen_saved_color_texture_.get())
      offscreen_saved_color_texture_->Destroy();
    if (offscreen_resolved_frame_buffer_.get())
      offscreen_resolved_frame_buffer_->Destroy();
    if (offscreen_resolved_color_texture_.get())
      offscreen_resolved_color_texture_->Destroy();
  } else {
    if (offscreen_target_frame_buffer_.get())
      offscreen_target_frame_buffer_->Invalidate();
    if (offscreen_target_color_texture_.get())
      offscreen_target_color_texture_->Invalidate();
    if (offscreen_target_color_render_buffer_.get())
      offscreen_target_color_render_buffer_->Invalidate();
    if (offscreen_target_depth_render_buffer_.get())
      offscreen_target_depth_render_buffer_->Invalidate();
    if (offscreen_target_stencil_render_buffer_.get())
      offscreen_target_stencil_render_buffer_->Invalidate();
    if (offscreen_saved_frame_buffer_.get())
      offscreen_saved_frame_buffer_->Invalidate();
    if (offscreen_saved_color_texture_.get())
      offscreen_saved_color_texture_->Invalidate();
    if (offscreen_resolved_frame_buffer_.get())
      offscreen_resolved_frame_buffer_->Invalidate();
    if (offscreen_resolved_color_texture_.get())
      offscreen_resolved_color_texture_->Invalidate();
  }

  if (query_manager_.get()) {
    query_manager_->Destroy(have_context);
    query_manager_.reset();
  }

  if (group_) {
    group_->Destroy(have_context);
    group_ = NULL;
  }

  if (context_.get()) {
    context_->ReleaseCurrent(NULL);
    context_ = NULL;
  }

  offscreen_target_frame_buffer_.reset();
  offscreen_target_color_texture_.reset();
  offscreen_target_color_render_buffer_.reset();
  offscreen_target_depth_render_buffer_.reset();
  offscreen_target_stencil_render_buffer_.reset();
  offscreen_saved_frame_buffer_.reset();
  offscreen_saved_color_texture_.reset();
  offscreen_resolved_frame_buffer_.reset();
  offscreen_resolved_color_texture_.reset();

#if defined(OS_MACOSX)
  for (TextureToIOSurfaceMap::iterator it = texture_to_io_surface_map_.begin();
       it != texture_to_io_surface_map_.end(); ++it) {
    CFRelease(it->second);
  }
  texture_to_io_surface_map_.clear();
#endif
}
