void GLES2DecoderImpl::DoDeleteSharedIds(
    GLuint namespace_id, GLsizei n, const GLuint* ids) {
  IdAllocator* id_allocator = group_->GetIdAllocator(namespace_id);
  for (GLsizei ii = 0; ii < n; ++ii) {
    id_allocator->FreeID(ids[ii]);
  }
}
