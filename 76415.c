bool GLES2DecoderImpl::GetUniformTypeByLocation(
    GLint location, const char* function_name, GLenum* type) {
  if (!CheckCurrentProgramForUniform(location, function_name)) {
    return false;
  }
  if (!current_program_->GetUniformTypeByLocation(location, type)) {
    SetGLError(GL_INVALID_OPERATION,
               (std::string(function_name) + ": program not linked").c_str());
    return false;
  }
  return true;
}
