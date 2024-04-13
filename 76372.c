void GLES2DecoderImpl::DoCompileShader(GLuint client_id) {
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      client_id, "glCompileShader");
  if (!info) {
    return;
  }
  const char* shader_src = info->source().c_str();
#if defined(GLES2_GPU_SERVICE_TRANSLATE_SHADER)
  if (use_shader_translator_) {
    int dbg_options = 0;
    ShHandle compiler = info->shader_type() == GL_VERTEX_SHADER ?
        vertex_compiler_ : fragment_compiler_;

    if (!ShCompile(compiler, &shader_src, 1, EShOptNone, dbg_options)) {
      info->SetStatus(false, ShGetInfoLog(compiler));
      return;
    }
    shader_src = ShGetObjectCode(compiler);
  }
#endif  // GLES2_GPU_SERVICE_TRANSLATE_SHADER

  glShaderSource(info->service_id(), 1, &shader_src, NULL);
  glCompileShader(info->service_id());
  GLint status = GL_FALSE;
  glGetShaderiv(info->service_id(),  GL_COMPILE_STATUS, &status);
  if (status) {
    info->SetStatus(true, "");
  } else {
    GLint len = 0;
    glGetShaderiv(info->service_id(), GL_INFO_LOG_LENGTH, &len);
    scoped_array<char> temp(new char[len]);
    glGetShaderInfoLog(info->service_id(), len, &len, temp.get());
    info->SetStatus(false, std::string(temp.get(), len));
  }
};
