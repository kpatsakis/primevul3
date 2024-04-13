bool VertexAttribManager::Enable(GLuint index, bool enable) {
  if (index >= max_vertex_attribs_) {
    return false;
  }
  VertexAttribInfo& info = vertex_attrib_infos_[index];
  if (info.enabled() != enable) {
    info.set_enabled(enable);
    info.SetList(enable ? &enabled_vertex_attribs_ : &disabled_vertex_attribs_);
  }
  return true;
}
