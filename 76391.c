void GLES2DecoderImpl::DoLinkProgram(GLuint program) {
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glLinkProgram");
  if (!info) {
    return;
  }
  if (!info->CanLink()) {
    return;
  }
  glLinkProgram(info->service_id());
  GLint success = 0;
  glGetProgramiv(info->service_id(), GL_LINK_STATUS, &success);
  if (success) {
    info->Update();
  } else {
    info->Reset();
  }
};
