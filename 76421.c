error::Error GLES2DecoderImpl::HandleGenSharedIds(
    uint32 immediate_data_size, const gles2::GenSharedIds& c) {
  GLuint namespace_id = static_cast<GLuint>(c.namespace_id);
  GLuint id_offset = static_cast<GLuint>(c.id_offset);
  GLsizei n = static_cast<GLsizei>(c.n);
  uint32 data_size;
  if (!SafeMultiplyUint32(n, sizeof(GLuint), &data_size)) {
    return error::kOutOfBounds;
  }
  GLuint* ids = GetSharedMemoryAs<GLuint*>(
      c.ids_shm_id, c.ids_shm_offset, data_size);
  if (n < 0) {
    SetGLError(GL_INVALID_VALUE, "GenSharedIds: n < 0");
    return error::kNoError;
  }
  if (ids == NULL) {
    return error::kOutOfBounds;
  }
  DoGenSharedIds(namespace_id, id_offset, n, ids);
  return error::kNoError;
}
