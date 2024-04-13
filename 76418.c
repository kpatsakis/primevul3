error::Error GLES2DecoderImpl::HandleDeleteProgram(
    uint32 immediate_data_size, const gles2::DeleteProgram& c) {
  GLuint client_id = c.program;
  if (client_id) {
    ProgramManager::ProgramInfo* info = GetProgramInfo(client_id);
    if (info) {
      glDeleteProgram(info->service_id());
      RemoveProgramInfo(client_id);
    } else {
      SetGLError(GL_INVALID_VALUE, "glDeleteProgram: unknown program");
    }
  }
  return error::kNoError;
}
