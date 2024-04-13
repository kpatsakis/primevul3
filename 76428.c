error::Error GLES2DecoderImpl::HandleRegisterSharedIds(
    uint32 immediate_data_size, const gles2::RegisterSharedIds& c) {
  GLuint namespace_id = static_cast<GLuint>(c.namespace_id);
  GLsizei n = static_cast<GLsizei>(c.n);
  uint32 data_size;
  if (!SafeMultiplyUint32(n, sizeof(GLuint), &data_size)) {
    return error::kOutOfBounds;
  }
  GLuint* ids = GetSharedMemoryAs<GLuint*>(
      c.ids_shm_id, c.ids_shm_offset, data_size);
  if (n < 0) {
    SetGLError(GL_INVALID_VALUE, "RegisterSharedIds: n < 0");
    return error::kNoError;
  }
  if (ids == NULL) {
    return error::kOutOfBounds;
  }
  DoRegisterSharedIds(namespace_id, n, ids);
  return error::kNoError;
}
