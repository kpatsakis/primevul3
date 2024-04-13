void GLES2DecoderImpl::DeleteTexturesHelper(
    GLsizei n, const GLuint* client_ids) {
  bool supports_seperate_framebuffer_binds =
     feature_info_->feature_flags().chromium_framebuffer_multisample;
  for (GLsizei ii = 0; ii < n; ++ii) {
    TextureManager::TextureInfo* texture = GetTextureInfo(client_ids[ii]);
    if (texture && !texture->IsDeleted()) {
      if (texture->IsAttachedToFramebuffer()) {
        state_dirty_ = true;
      }
      for (size_t jj = 0; jj < group_->max_texture_units(); ++jj) {
        texture_units_[ii].Unbind(texture);
      }
      if (supports_seperate_framebuffer_binds) {
        if (bound_read_framebuffer_) {
          bound_read_framebuffer_->UnbindTexture(GL_READ_FRAMEBUFFER, texture);
        }
        if (bound_draw_framebuffer_) {
          bound_draw_framebuffer_->UnbindTexture(GL_DRAW_FRAMEBUFFER, texture);
        }
      } else {
        if (bound_draw_framebuffer_) {
          bound_draw_framebuffer_->UnbindTexture(GL_FRAMEBUFFER, texture);
        }
      }
      GLuint service_id = texture->service_id();
      if (texture->IsStreamTexture() && stream_texture_manager_) {
        stream_texture_manager_->DestroyStreamTexture(service_id);
      }
#if defined(OS_MACOSX)
      if (texture->target() == GL_TEXTURE_RECTANGLE_ARB) {
        ReleaseIOSurfaceForTexture(service_id);
      }
#endif
      RemoveTextureInfo(client_ids[ii]);
    }
  }
}
