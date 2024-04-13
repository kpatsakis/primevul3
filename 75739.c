GLvoid StubGLBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                           const void* data) {
  glBufferSubData(target, offset, size, data);
}
