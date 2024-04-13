void GLES2DecoderImpl::DoUniform1iv(
    GLint location, GLsizei count, const GLint *value) {
  if (!CheckCurrentProgramForUniform(location, "glUniform1iv")) {
    return;
  }
  current_program_->SetSamplers(location, count, value);
  glUniform1iv(location, count, value);
}
