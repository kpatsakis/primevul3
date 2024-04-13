GLvoid StubGLBufferData(GLenum target, GLsizeiptr size, const void* data,
                        GLenum usage) {
  glBufferData(target, size, data, usage);
}
