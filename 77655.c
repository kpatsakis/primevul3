error::Error GLES2DecoderImpl::HandleDrawArraysInstancedANGLE(
    uint32 immediate_data_size, const gles2::DrawArraysInstancedANGLE& c) {
  if (!feature_info_->feature_flags().angle_instanced_arrays) {
    SetGLError(GL_INVALID_OPERATION,
               "glDrawArraysInstancedANGLE: function not available");
    return error::kNoError;
  }
  return DoDrawArrays(true,
                      static_cast<GLenum>(c.mode),
                      static_cast<GLint>(c.first),
                      static_cast<GLsizei>(c.count),
                      static_cast<GLsizei>(c.primcount));
}
