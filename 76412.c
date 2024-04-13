  ShaderManager::ShaderInfo* GetShaderInfo(GLuint client_id) {
    ShaderManager::ShaderInfo* info =
        shader_manager()->GetShaderInfo(client_id);
    return (info && !info->IsDeleted()) ? info : NULL;
  }
