void GLES2DecoderImpl::DoCompileShader(GLuint client_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCompileShader");
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      client_id, "glCompileShader");
  if (!info) {
    return;
  }
  const char* shader_src = info->source() ? info->source()->c_str() : "";
  ShaderTranslator* translator = NULL;
  if (use_shader_translator_) {
    translator = info->shader_type() == GL_VERTEX_SHADER ?
        vertex_translator_.get() : fragment_translator_.get();

    if (!translator->Translate(shader_src)) {
      info->SetStatus(false, translator->info_log(), NULL);
      return;
    }
    shader_src = translator->translated_shader();
    if (!feature_info_->feature_flags().angle_translated_shader_source)
      info->UpdateTranslatedSource(shader_src);
  }

  glShaderSource(info->service_id(), 1, &shader_src, NULL);
  glCompileShader(info->service_id());
  if (feature_info_->feature_flags().angle_translated_shader_source) {
    GLint max_len = 0;
    glGetShaderiv(info->service_id(),
                  GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE,
                  &max_len);
    scoped_array<char> temp(new char[max_len]);
    GLint len = 0;
    glGetTranslatedShaderSourceANGLE(
        info->service_id(), max_len, &len, temp.get());
    DCHECK(max_len == 0 || len < max_len);
    DCHECK(len == 0 || temp[len] == '\0');
    info->UpdateTranslatedSource(temp.get());
  }

  GLint status = GL_FALSE;
  glGetShaderiv(info->service_id(),  GL_COMPILE_STATUS, &status);
  if (status) {
    info->SetStatus(true, "", translator);
  } else {
    LOG_IF(ERROR, use_shader_translator_)
        << "Shader translator allowed/produced an invalid shader.";
    GLint max_len = 0;
    glGetShaderiv(info->service_id(), GL_INFO_LOG_LENGTH, &max_len);
    scoped_array<char> temp(new char[max_len]);
    GLint len = 0;
    glGetShaderInfoLog(info->service_id(), max_len, &len, temp.get());
    DCHECK(max_len == 0 || len < max_len);
    DCHECK(len == 0 || temp[len] == '\0');
    info->SetStatus(false, std::string(temp.get(), len).c_str(), NULL);
  }
};
