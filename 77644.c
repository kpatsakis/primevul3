void GLES2DecoderImpl::DoVertexAttrib4fv(GLuint index, const GLfloat* v) {
  VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_->GetVertexAttribInfo(index);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glVertexAttrib4fv: index out of range");
    return;
  }
  VertexAttribManager::VertexAttribInfo::Vec4 value;
  value.v[0] = v[0];
  value.v[1] = v[1];
  value.v[2] = v[2];
  value.v[3] = v[3];
  info->set_value(value);
  glVertexAttrib4fv(index, v);
}
