void GLES2DecoderImpl::DoVertexAttrib2fv(GLuint index, const GLfloat* v) {
  VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_->GetVertexAttribInfo(index);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glVertexAttrib2fv: index out of range");
    return;
  }
  VertexAttribManager::VertexAttribInfo::Vec4 value;
  value.v[0] = v[0];
  value.v[1] = v[1];
  value.v[2] = 0.0f;
  value.v[3] = 1.0f;
  info->set_value(value);
  glVertexAttrib2fv(index, v);
}
