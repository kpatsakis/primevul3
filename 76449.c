bool GLES2DecoderImpl::UpdateOffscreenFrameBufferSize() {
  if (offscreen_target_color_texture_->size() == pending_offscreen_size_)
    return true;

  if (!offscreen_target_color_texture_->AllocateStorage(
      pending_offscreen_size_)) {
    return false;
  }

  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 &&
      !depth24_stencil8_oes_supported_) {
    if (!offscreen_target_depth_render_buffer_->AllocateStorage(
        pending_offscreen_size_, GL_DEPTH_COMPONENT16)) {
      return false;
    }

    if (!offscreen_target_stencil_render_buffer_->AllocateStorage(
        pending_offscreen_size_, GL_STENCIL_INDEX8)) {
      return false;
    }
  } else {
    if (!offscreen_target_depth_render_buffer_->AllocateStorage(
        pending_offscreen_size_, GL_DEPTH24_STENCIL8)) {
      return false;
    }
  }

  offscreen_target_frame_buffer_->AttachRenderTexture(
      offscreen_target_color_texture_.get());
  offscreen_target_frame_buffer_->AttachRenderBuffer(
      GL_DEPTH_ATTACHMENT,
      offscreen_target_depth_render_buffer_.get());
  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 &&
      !depth24_stencil8_oes_supported_) {
    offscreen_target_frame_buffer_->AttachRenderBuffer(
        GL_STENCIL_ATTACHMENT,
        offscreen_target_stencil_render_buffer_.get());
  } else {
    offscreen_target_frame_buffer_->AttachRenderBuffer(
        GL_STENCIL_ATTACHMENT,
        offscreen_target_depth_render_buffer_.get());
  }
  if (offscreen_target_frame_buffer_->CheckStatus() !=
      GL_FRAMEBUFFER_COMPLETE) {
    return false;
  }

  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    ScopedDefaultGLContext scoped_context(this);
    glBindFramebufferEXT(GL_FRAMEBUFFER,
                         offscreen_target_frame_buffer_->id());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

    if (glGetError() != GL_NO_ERROR)
      return false;
  }

  if (parent_) {
    offscreen_saved_color_texture_->AllocateStorage(pending_offscreen_size_);

    GLuint service_id = offscreen_saved_color_texture_->id();
    GLuint client_id;
    CHECK(parent_->texture_manager()->GetClientId(service_id, &client_id));
    TextureManager::TextureInfo* info = parent_->GetTextureInfo(client_id);
    DCHECK(info);

    texture_manager()->SetLevelInfo(
        info,
        GL_TEXTURE_2D,
        0,  // level
        GL_RGBA,
        pending_offscreen_size_.width(),
        pending_offscreen_size_.height(),
        1,  // depth
        0,  // border
        GL_RGBA,
        GL_UNSIGNED_BYTE);

    offscreen_target_frame_buffer_->AttachRenderTexture(
        offscreen_saved_color_texture_.get());
    if (offscreen_target_frame_buffer_->CheckStatus() !=
        GL_FRAMEBUFFER_COMPLETE) {
      return false;
    }

    if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
      ScopedDefaultGLContext scoped_context(this);
      glBindFramebufferEXT(GL_FRAMEBUFFER,
                           offscreen_target_frame_buffer_->id());
      glClear(GL_COLOR_BUFFER_BIT);
      glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

      if (glGetError() != GL_NO_ERROR)
        return false;
    }

    offscreen_target_frame_buffer_->AttachRenderTexture(
        offscreen_target_color_texture_.get());
  }

  return true;
}
