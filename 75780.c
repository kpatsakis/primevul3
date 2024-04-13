GLvoid StubGLRenderbufferStorageMultisample(GLenum target, GLsizei samples,
                                            GLenum internalformat,
                                            GLsizei width, GLsizei height) {
  glRenderbufferStorageMultisampleEXT(target, samples, internalformat, width,
                                      height);
}
