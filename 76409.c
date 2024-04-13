gfx::Size GLES2DecoderImpl::GetBoundFrameBufferSize() {
  if (bound_framebuffer_ != 0) {
    int width = 0;
    int height = 0;

    GLint fb_type = 0;
    glGetFramebufferAttachmentParameterivEXT(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
        &fb_type);
    switch (fb_type) {
      case GL_RENDERBUFFER:
        {
          GLint renderbuffer_id = 0;
          glGetFramebufferAttachmentParameterivEXT(
              GL_FRAMEBUFFER,
              GL_COLOR_ATTACHMENT0,
              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
              &renderbuffer_id);
          if (renderbuffer_id != 0) {
            glGetRenderbufferParameterivEXT(
                GL_RENDERBUFFER,
                GL_RENDERBUFFER_WIDTH,
                &width);
            glGetRenderbufferParameterivEXT(
                GL_RENDERBUFFER,
                GL_RENDERBUFFER_HEIGHT,
                &height);
          }
          break;
        }
      case GL_TEXTURE:
        {
          GLint texture_id = 0;
          glGetFramebufferAttachmentParameterivEXT(
              GL_FRAMEBUFFER,
              GL_COLOR_ATTACHMENT0,
              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
              &texture_id);
          if (texture_id != 0) {
            GLuint client_id = 0;
            if (texture_manager()->GetClientId(texture_id, &client_id)) {
              TextureManager::TextureInfo* texture_info =
                 GetTextureInfo(client_id);
              if (texture_info) {
                GLint level = 0;
                GLint face = 0;
                glGetFramebufferAttachmentParameterivEXT(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
                    &level);
                glGetFramebufferAttachmentParameterivEXT(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
                    &face);
                texture_info->GetLevelSize(
                    face ? face : GL_TEXTURE_2D, level, &width, &height);
              }
            }
          }
          break;
        }
      default:
        break;
    }

    return gfx::Size(width, height);
  } else if (offscreen_target_color_texture_.get()) {
    return offscreen_target_color_texture_->size();
  } else {
    return context_->GetSize();
  }
}
