error::Error GLES2DecoderImpl::ShaderSourceHelper(
    GLuint client_id, const char* data, uint32 data_size) {
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      client_id, "glShaderSource");
  if (!info) {
    return error::kNoError;
  }
  info->Update(std::string(data, data + data_size));
  return error::kNoError;
}
