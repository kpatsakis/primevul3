void GLES2DecoderImpl::DoUniform1iv(
    GLint fake_location, GLsizei count, const GLint *value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(
      fake_location, "glUniform1iv", &real_location, &type, &count)) {
    return;
  }
  if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE ||
      type == GL_SAMPLER_EXTERNAL_OES) {
    current_program_->SetSamplers(fake_location, count, value);
  }
  glUniform1iv(real_location, count, value);
}
