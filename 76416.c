  VertexAttribInfo* GetVertexAttribInfo(GLuint index) {
    if (index < max_vertex_attribs_) {
      return &vertex_attrib_infos_[index];
    }
    return NULL;
  }
