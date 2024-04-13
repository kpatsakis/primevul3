void GLES2DecoderImpl::DoDisableVertexAttribArray(GLuint index) {
  if (vertex_attrib_manager_.Enable(index, false)) {
    if (index != 0) {
      glDisableVertexAttribArray(index);
    }
  } else {
    SetGLError(GL_INVALID_VALUE,
               "glDisableVertexAttribArray: index out of range");
  }
}
