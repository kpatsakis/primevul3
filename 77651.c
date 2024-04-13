error::Error GLES2DecoderImpl::HandleBeginQueryEXT(
    uint32 immediate_data_size, const gles2::BeginQueryEXT& c) {
  GLenum target = static_cast<GLenum>(c.target);
  GLuint client_id = static_cast<GLuint>(c.id);
  int32 sync_shm_id = static_cast<int32>(c.sync_data_shm_id);
  uint32 sync_shm_offset = static_cast<uint32>(c.sync_data_shm_offset);

  switch (target) {
    case GL_COMMANDS_ISSUED_CHROMIUM:
      break;
    default:
      if (!feature_info_->feature_flags().occlusion_query_boolean) {
        SetGLError(GL_INVALID_OPERATION, "glBeginQueryEXT: not enabled");
        return error::kNoError;
      }
      break;
  }

  if (current_query_) {
    SetGLError(
        GL_INVALID_OPERATION, "glBeginQueryEXT: query already in progress");
    return error::kNoError;
  }

  if (client_id == 0) {
    SetGLError(GL_INVALID_OPERATION, "glBeginQueryEXT: id is 0");
    return error::kNoError;
  }

  QueryManager::Query* query = query_manager_->GetQuery(client_id);
  if (!query) {
    query = query_manager_->CreateQuery(
        target, client_id, sync_shm_id, sync_shm_offset);
  }

  if (query->target() != target) {
    SetGLError(GL_INVALID_OPERATION, "glBeginQueryEXT: target does not match");
    return error::kNoError;
  } else if (query->shm_id() != sync_shm_id ||
             query->shm_offset() != sync_shm_offset) {
    DLOG(ERROR) << "Shared memory used by query not the same as before";
    return error::kInvalidArguments;
  }

  if (!query_manager_->BeginQuery(query)) {
    return error::kOutOfBounds;
  }

  current_query_ = query;
  return error::kNoError;
}
