error::Error GLES2DecoderImpl::GetUniformLocationHelper(
    GLuint client_id, uint32 location_shm_id, uint32 location_shm_offset,
    const std::string& name_str) {
  if (!StringIsValidForGLES(name_str.c_str())) {
    SetGLError(GL_INVALID_VALUE, "glGetUniformLocation: Invalid character");
    return error::kNoError;
  }
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      client_id, "glUniformLocation");
  if (!info) {
    return error::kNoError;
  }
  if (!info->IsValid()) {
    SetGLError(GL_INVALID_OPERATION,
               "glGetUniformLocation: program not linked");
    return error::kNoError;
  }
  GLint* location = GetSharedMemoryAs<GLint*>(
      location_shm_id, location_shm_offset, sizeof(GLint));
  if (!location) {
    return error::kOutOfBounds;
  }
  if (*location != -1) {
    return error::kGenericError;
  }
  *location = program_manager()->SwizzleLocation(
      info->GetUniformFakeLocation(name_str));
  return error::kNoError;
}
