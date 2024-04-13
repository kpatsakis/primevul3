bool GLES2DecoderImpl::SimulateAttrib0(GLuint max_vertex_accessed) {
  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2)
    return false;

  const VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_.GetVertexAttribInfo(0);
  if (info->enabled() || !current_program_->GetAttribInfoByLocation(0)) {
    return false;
  }

  typedef VertexAttribManager::VertexAttribInfo::Vec4 Vec4;

  glBindBuffer(GL_ARRAY_BUFFER, attrib_0_buffer_id_);

  GLsizei num_vertices = max_vertex_accessed + 1;
  GLsizei size_needed = num_vertices * sizeof(Vec4);  // NOLINT
  if (size_needed > attrib_0_size_ ||
      info->value().v[0] != attrib_0_value_.v[0] ||
      info->value().v[1] != attrib_0_value_.v[1] ||
      info->value().v[2] != attrib_0_value_.v[2] ||
      info->value().v[3] != attrib_0_value_.v[3]) {
    scoped_array<Vec4> temp(new Vec4[num_vertices]);
    for (GLsizei ii = 0; ii < num_vertices; ++ii) {
      temp[ii] = info->value();
    }
    glBufferData(
        GL_ARRAY_BUFFER,
        size_needed,
        &temp[0].v[0],
        GL_DYNAMIC_DRAW);
    attrib_0_value_ = info->value();
    attrib_0_size_ = size_needed;
  }

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

  return true;
}
