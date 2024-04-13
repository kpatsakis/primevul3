bool GLES2DecoderImpl::GetHelper(
    GLenum pname, GLint* params, GLsizei* num_written) {
  DCHECK(num_written);
  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    switch (pname) {
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
      *num_written = 1;
      if (params) {
        *params = GL_RGBA;  // TODO(gman): get correct format.
      }
      return true;
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:
      *num_written = 1;
      if (params) {
        *params = GL_UNSIGNED_BYTE;  // TODO(gman): get correct type.
      }
      return true;
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
      *num_written = 1;
      if (params) {
        *params = group_->max_fragment_uniform_vectors();
      }
      return true;
    case GL_MAX_VARYING_VECTORS:
      *num_written = 1;
      if (params) {
        *params = group_->max_varying_vectors();
      }
      return true;
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
      *num_written = 1;
      if (params) {
        *params = group_->max_vertex_uniform_vectors();
      }
      return true;
    }
  }
  switch (pname) {
    case GL_COMPRESSED_TEXTURE_FORMATS:
      *num_written = 0;
      return true;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
      *num_written = 1;
      if (params) {
        *params = 0;  // We don't support compressed textures.
      }
      return true;
    case GL_NUM_SHADER_BINARY_FORMATS:
      *num_written = 1;
      if (params) {
        *params = 0;  // We don't support binary shader formats.
      }
      return true;
    case GL_SHADER_BINARY_FORMATS:
      *num_written = 0;
      return true;  // We don't support binary shader format.s
    case GL_SHADER_COMPILER:
      *num_written = 1;
      if (params) {
        *params = GL_TRUE;
      }
      return true;
    case GL_ARRAY_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        if (bound_array_buffer_) {
          GLuint client_id = 0;
          buffer_manager()->GetClientId(bound_array_buffer_->service_id(),
                                        &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        if (bound_element_array_buffer_) {
          GLuint client_id = 0;
          buffer_manager()->GetClientId(
              bound_element_array_buffer_->service_id(),
              &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_FRAMEBUFFER_BINDING:
      *num_written = 1;
      if (params) {
        if (bound_framebuffer_) {
          GLuint client_id = 0;
          framebuffer_manager()->GetClientId(
              bound_framebuffer_->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_RENDERBUFFER_BINDING:
      *num_written = 1;
      if (params) {
        if (bound_renderbuffer_) {
          GLuint client_id = 0;
          renderbuffer_manager()->GetClientId(
              bound_renderbuffer_->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_CURRENT_PROGRAM:
      *num_written = 1;
      if (params) {
        if (current_program_) {
          GLuint client_id = 0;
          program_manager()->GetClientId(
              current_program_->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_2D:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = texture_units_[active_texture_unit_];
        if (unit.bound_texture_2d) {
          GLuint client_id = 0;
          texture_manager()->GetClientId(
              unit.bound_texture_2d->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_CUBE_MAP:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = texture_units_[active_texture_unit_];
        if (unit.bound_texture_cube_map) {
          GLuint client_id = 0;
          texture_manager()->GetClientId(
              unit.bound_texture_cube_map->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    default:
      *num_written = util_.GLGetNumValuesReturned(pname);
      if (params) {
        glGetIntegerv(pname, params);
      }
      return true;
  }
}
