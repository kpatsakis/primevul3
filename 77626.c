void GLES2DecoderImpl::DoGetShaderiv(
    GLuint shader, GLenum pname, GLint* params) {
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      shader, "glGetShaderiv");
  if (!info) {
    return;
  }
  switch (pname) {
    case GL_SHADER_SOURCE_LENGTH:
      *params = info->source() ? info->source()->size() + 1 : 0;
      return;
    case GL_COMPILE_STATUS:
      *params = compile_shader_always_succeeds_ ? true : info->IsValid();
      return;
    case GL_INFO_LOG_LENGTH:
      *params = info->log_info() ? info->log_info()->size() + 1 : 0;
      return;
    case GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE:
      *params = info->translated_source() ?
          info->translated_source()->size() + 1 : 0;
      return;
    default:
      break;
  }
  glGetShaderiv(info->service_id(), pname, params);
}
