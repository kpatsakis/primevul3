error::Error GLES2DecoderImpl::HandleGetVertexAttribPointerv(
    uint32 immediate_data_size, const gles2::GetVertexAttribPointerv& c) {
  GLuint index = static_cast<GLuint>(c.index);
  GLenum pname = static_cast<GLenum>(c.pname);
  typedef gles2::GetVertexAttribPointerv::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
        c.pointer_shm_id, c.pointer_shm_offset, Result::ComputeSize(1));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  if (!validators_->vertex_pointer.IsValid(pname)) {
    SetGLError(GL_INVALID_ENUM,
               "glGetVertexAttribPointerv: pname GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (index >= group_->max_vertex_attribs()) {
    SetGLError(GL_INVALID_VALUE,
               "glGetVertexAttribPointerv: index out of range.");
    return error::kNoError;
  }
  result->SetNumResults(1);
  *result->GetData() =
      vertex_attrib_manager_->GetVertexAttribInfo(index)->offset();
  return error::kNoError;
}
