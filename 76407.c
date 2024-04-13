void GLES2DecoderImpl::DoValidateProgram(GLuint program_client_id) {
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program_client_id, "glValidateProgram");
  if (!info) {
    return;
  }
  if (!info->CanLink()) {
    info->set_log_info("Missing Shader");
    return;
  }
  glValidateProgram(info->service_id());
  info->UpdateLogInfo();
}
