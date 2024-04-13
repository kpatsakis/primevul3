void GLES2DecoderImpl::DoVertexAttrib3f(
    GLuint index, GLfloat v0, GLfloat v1, GLfloat v2) {
  VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_->GetVertexAttribInfo(index);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glVertexAttrib3f: index out of range");
    return;
  }
  VertexAttribManager::VertexAttribInfo::Vec4 value;
  value.v[0] = v0;
  value.v[1] = v1;
  value.v[2] = v2;
  value.v[3] = 1.0f;
  info->set_value(value);
  glVertexAttrib3f(index, v0, v1, v2);
}
