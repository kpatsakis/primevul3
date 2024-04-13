bool GLES2DecoderImpl::PrepForSetUniformByLocation(
    GLint fake_location, const char* function_name,
    GLint* real_location, GLenum* type, GLsizei* count) {
  DCHECK(type);
  DCHECK(count);
  DCHECK(real_location);
  if (!CheckCurrentProgramForUniform(fake_location, function_name)) {
    return false;
  }
  GLint array_index = -1;
  const ProgramManager::ProgramInfo::UniformInfo* info =
      current_program_->GetUniformInfoByFakeLocation(
          fake_location, real_location, &array_index);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               (std::string(function_name) + ": unknown location").c_str());
    return false;
  }
  if (*count > 1 && !info->is_array) {
    SetGLError(
        GL_INVALID_OPERATION,
        (std::string(function_name) + ": count > 1 for non-array").c_str());
    return false;
  }
  *count = std::min(info->size - array_index, *count);
  if (*count <= 0) {
    return false;
  }
  *type = info->type;
  return true;
}
