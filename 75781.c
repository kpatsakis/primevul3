GLvoid StubGLShaderSource(GLuint shader, GLsizei count, const char** str,
                          const GLint* length) {
  glShaderSource(shader, count, str, length);
}
