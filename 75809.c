GLvoid StubGLVertexAttribPointer(GLuint indx, GLint size, GLenum type,
                                 GLboolean normalized, GLsizei stride,
                                 const void* ptr) {
  glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}
