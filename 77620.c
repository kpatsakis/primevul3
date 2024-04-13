error::Error GLES2DecoderImpl::DoDrawElements(bool instanced,
                                              GLenum mode,
                                              GLsizei count,
                                              GLenum type,
                                              int32 offset,
                                              GLsizei primcount) {
  if (!bound_element_array_buffer_) {
    SetGLError(GL_INVALID_OPERATION,
               "glDrawElements: No element array buffer bound");
    return error::kNoError;
  }

  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawElements: count < 0");
    return error::kNoError;
  }
  if (offset < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawElements: offset < 0");
    return error::kNoError;
  }
  if (!validators_->draw_mode.IsValid(mode)) {
    SetGLError(GL_INVALID_ENUM, "glDrawElements: mode GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->index_type.IsValid(type)) {
    SetGLError(GL_INVALID_ENUM, "glDrawElements: type GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (primcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawElements: primcount < 0");
    return error::kNoError;
  }

  if (!CheckBoundFramebuffersValid("glDrawElements")) {
    return error::kNoError;
  }

  if (count == 0 || (instanced && primcount == 0)) {
    return error::kNoError;
  }

  GLuint max_vertex_accessed;
  if (!bound_element_array_buffer_->GetMaxValueForRange(
      offset, count, type, &max_vertex_accessed)) {
    SetGLError(GL_INVALID_OPERATION,
               "glDrawElements: range out of bounds for buffer");
    return error::kNoError;
  }

  if (IsDrawValid(max_vertex_accessed, primcount)) {
    if (!ClearUnclearedTextures()) {
      SetGLError(GL_INVALID_VALUE, "glDrawElements: out of memory");
      return error::kNoError;
    }
    bool simulated_attrib_0 = false;
    if (!SimulateAttrib0(max_vertex_accessed, &simulated_attrib_0)) {
      return error::kNoError;
    }
    bool simulated_fixed_attribs = false;
    if (SimulateFixedAttribs(max_vertex_accessed, &simulated_fixed_attribs,
                             primcount)) {
      bool textures_set = SetBlackTextureForNonRenderableTextures();
      ApplyDirtyState();
      const GLvoid* indices = reinterpret_cast<const GLvoid*>(offset);
      if (!instanced) {
        glDrawElements(mode, count, type, indices);
      } else {
        glDrawElementsInstancedANGLE(mode, count, type, indices, primcount);
      }
      ProcessPendingQueries();
      if (textures_set) {
        RestoreStateForNonRenderableTextures();
      }
      if (simulated_fixed_attribs) {
        RestoreStateForSimulatedFixedAttribs();
      }
    }
    if (simulated_attrib_0) {
      RestoreStateForSimulatedAttrib0();
    }
    if (WasContextLost()) {
      LOG(ERROR) << "  GLES2DecoderImpl: Context lost during DrawElements.";
      return error::kLostContext;
    }
  }
  return error::kNoError;
}
