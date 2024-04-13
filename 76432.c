void VertexAttribManager::Initialize(uint32 max_vertex_attribs) {
  max_vertex_attribs_ = max_vertex_attribs;
  vertex_attrib_infos_.reset(
      new VertexAttribInfo[max_vertex_attribs]);
  for (uint32 vv = 0; vv < max_vertex_attribs; ++vv) {
    vertex_attrib_infos_[vv].set_index(vv);
    vertex_attrib_infos_[vv].SetList(&disabled_vertex_attribs_);
  }
}
