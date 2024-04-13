bool GLES2DecoderImpl::GetUniformSetup(
    GLuint program, GLint location,
    uint32 shm_id, uint32 shm_offset,
    error::Error* error, GLuint* service_id, void** result_pointer,
    GLenum* result_type) {
  DCHECK(error);
  DCHECK(service_id);
  DCHECK(result_pointer);
  DCHECK(result_type);
  *error = error::kNoError;
  SizedResult<GLint>* result;
  result = GetSharedMemoryAs<SizedResult<GLint>*>(
      shm_id, shm_offset, SizedResult<GLint>::ComputeSize(0));
  if (!result) {
    *error = error::kOutOfBounds;
    return false;
  }
  *result_pointer = result;
  result->SetNumResults(0);
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glGetUniform");
  if (!info) {
    return false;
  }
  if (!info->IsValid()) {
    SetGLError(GL_INVALID_OPERATION, "glGetUniform: program not linked");
    return false;
  }
  *service_id = info->service_id();
  GLenum type;
  if (!info->GetUniformTypeByLocation(location, &type)) {
    SetGLError(GL_INVALID_OPERATION, "glGetUniform: unknown location");
    return false;
  }
  GLsizei size = GLES2Util::GetGLDataTypeSizeForUniforms(type);
  if (size == 0) {
    SetGLError(GL_INVALID_OPERATION, "glGetUniform: unknown type");
    return false;
  }
  result = GetSharedMemoryAs<SizedResult<GLint>*>(
      shm_id, shm_offset, SizedResult<GLint>::ComputeSizeFromBytes(size));
  if (!result) {
    *error = error::kOutOfBounds;
    return false;
  }
  result->size = size;
  *result_type = type;
  return true;
}
