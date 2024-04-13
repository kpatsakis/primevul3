error::Error GLES2DecoderImpl::DoDrawArrays(bool instanced,
                                            GLenum mode,
                                            GLint first,
                                            GLsizei count,
                                            GLsizei primcount) {
  if (!validators_->draw_mode.IsValid(mode)) {
    SetGLError(GL_INVALID_ENUM, "glDrawArrays: mode GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawArrays: count < 0");
    return error::kNoError;
  }
  if (primcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawArrays: primcount < 0");
    return error::kNoError;
  }
  if (!CheckBoundFramebuffersValid("glDrawArrays")) {
    return error::kNoError;
  }
  if (first < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawArrays: first < 0");
    return error::kNoError;
  }

  if (count == 0 || (instanced && primcount == 0)) {
    return error::kNoError;
  }

  GLuint max_vertex_accessed = first + count - 1;
  if (IsDrawValid(max_vertex_accessed, primcount)) {
    if (!ClearUnclearedTextures()) {
      SetGLError(GL_INVALID_VALUE, "glDrawArrays: out of memory");
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
      if (!instanced) {
        glDrawArrays(mode, first, count);
      } else {
        glDrawArraysInstancedANGLE(mode, first, count, primcount);
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
      LOG(ERROR) << "  GLES2DecoderImpl: Context lost during DrawArrays.";
      return error::kLostContext;
    }
  }
  return error::kNoError;
}
