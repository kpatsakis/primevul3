GLvoid StubGLGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length,
                               char* infolog) {
  glGetProgramInfoLog(program, bufsize, length, infolog);
}
