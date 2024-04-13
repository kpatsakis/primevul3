GLvoid StubGLUniformMatrix4fv(GLint location, GLsizei count,
                              GLboolean transpose, const GLfloat* value) {
  glUniformMatrix4fv(location, count, transpose, value);
}
