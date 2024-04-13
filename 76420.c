error::Error GLES2DecoderImpl::HandleDeleteSharedIds(
    uint32 immediate_data_size, const gles2::DeleteSharedIds& c) {
  GLuint namespace_id = static_cast<GLuint>(c.namespace_id);
  GLsizei n = static_cast<GLsizei>(c.n);
  uint32 data_size;
  if (!SafeMultiplyUint32(n, sizeof(GLuint), &data_size)) {
    return error::kOutOfBounds;
  }
  const GLuint* ids = GetSharedMemoryAs<const GLuint*>(
      c.ids_shm_id, c.ids_shm_offset, data_size);
  if (n < 0) {
    SetGLError(GL_INVALID_VALUE, "DeleteSharedIds: n < 0");
    return error::kNoError;
  }
  if (ids == NULL) {
    return error::kOutOfBounds;
  }
  DoDeleteSharedIds(namespace_id, n, ids);
  return error::kNoError;
}
