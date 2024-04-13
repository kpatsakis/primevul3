bool GLES2DecoderImpl::IsDrawValid(GLuint max_vertex_accessed) {
  if (!current_program_ || current_program_->IsDeleted()) {
    return false;
  }
  const VertexAttribManager::VertexAttribInfoList& infos =
      vertex_attrib_manager_.GetEnabledVertexAttribInfos();
  for (VertexAttribManager::VertexAttribInfoList::const_iterator it =
           infos.begin(); it != infos.end(); ++it) {
    const VertexAttribManager::VertexAttribInfo* info = *it;
    const ProgramManager::ProgramInfo::VertexAttribInfo* attrib_info =
        current_program_->GetAttribInfoByLocation(info->index());
    if (attrib_info) {
      if (!info->CanAccess(max_vertex_accessed)) {
        SetGLError(GL_INVALID_OPERATION,
                   "glDrawXXX: attempt to access out of range vertices");
        return false;
      }
    } else {
      if (!info->buffer() || info->buffer()->IsDeleted()) {
        SetGLError(
            GL_INVALID_OPERATION,
            "glDrawXXX: attempt to render with no buffer attached to enabled "
            "attrib");
        return false;
      }
    }
  }
  return true;
}
