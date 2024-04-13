void GLES2DecoderImpl::DoRegisterSharedIds(
    GLuint namespace_id, GLsizei n, const GLuint* ids) {
  IdAllocator* id_allocator = group_->GetIdAllocator(namespace_id);
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (!id_allocator->MarkAsUsed(ids[ii])) {
      for (GLsizei jj = 0; jj < ii; ++jj) {
        id_allocator->FreeID(ids[jj]);
      }
      SetGLError(
          GL_INVALID_VALUE,
          "RegisterSharedIds: attempt to register id that already exists");
      return;
    }
  }
}
