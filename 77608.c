void GLES2DecoderImpl::DoBindAttribLocation(
    GLuint program, GLuint index, const char* name) {
  if (!StringIsValidForGLES(name)) {
    SetGLError(GL_INVALID_VALUE, "glBindAttribLocation: Invalid character");
    return;
  }
  if (ProgramManager::IsInvalidPrefix(name, strlen(name))) {
    SetGLError(GL_INVALID_OPERATION, "glBindAttribLocation: reserved prefix");
    return;
  }
  if (index >= group_->max_vertex_attribs()) {
    SetGLError(GL_INVALID_VALUE, "glBindAttribLocation: index out of range");
    return;
  }
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glBindAttribLocation");
  if (!info) {
    return;
  }
  info->SetAttribLocationBinding(name, static_cast<GLint>(index));
  glBindAttribLocation(info->service_id(), index, name);
}
