void GLES2DecoderImpl::DoVertexAttrib1f(GLuint index, GLfloat v0) {
  VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_->GetVertexAttribInfo(index);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glVertexAttrib1f: index out of range");
    return;
  }
  VertexAttribManager::VertexAttribInfo::Vec4 value;
  value.v[0] = v0;
  value.v[1] = 0.0f;
  value.v[2] = 0.0f;
  value.v[3] = 1.0f;
  info->set_value(value);
  glVertexAttrib1f(index, v0);
}
