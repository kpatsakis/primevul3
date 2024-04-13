GLvoid StubGLFramebufferTexture2D(GLenum target, GLenum attachment,
                                  GLenum textarget, GLuint texture,
                                  GLint level) {
  glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
}
