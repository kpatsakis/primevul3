void GLES2DecoderImpl::DoEnableVertexAttribArray(GLuint index) {
  if (vertex_attrib_manager_->Enable(index, true)) {
    glEnableVertexAttribArray(index);
  } else {
    SetGLError(GL_INVALID_VALUE,
               "glEnableVertexAttribArray: index out of range");
  }
}
