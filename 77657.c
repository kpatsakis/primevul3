error::Error GLES2DecoderImpl::HandleEndQueryEXT(
    uint32 immediate_data_size, const gles2::EndQueryEXT& c) {
  GLenum target = static_cast<GLenum>(c.target);
  uint32 submit_count = static_cast<GLuint>(c.submit_count);

  if (!current_query_) {
    SetGLError(GL_INVALID_OPERATION, "glEndQueryEXT: No active query");
    return error::kNoError;
  }
  if (current_query_->target() != target) {
    SetGLError(GL_INVALID_OPERATION,
               "glEndQueryEXT: target does not match active query");
    return error::kNoError;
  }

  if (!query_manager_->EndQuery(current_query_, submit_count)) {
    return error::kOutOfBounds;
  }

  current_query_ = NULL;
  return error::kNoError;
}
