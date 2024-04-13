GLvoid StubGLGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length,
                              char* infolog) {
  glGetShaderInfoLog(shader, bufsize, length, infolog);
}
