GLvoid StubGLReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                        GLenum format, GLenum type, void* pixels) {
  glReadPixels(x, y, width, height, format, type, pixels);
}
