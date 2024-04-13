void GLES2DecoderImpl::DoBufferSubData(
  GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data) {
  BufferManager::BufferInfo* info = GetBufferInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glBufferSubData: unknown buffer");
    return;
  }
  if (!info->SetRange(offset, size, data)) {
    SetGLError(GL_INVALID_VALUE, "glBufferSubData: out of range");
  } else {
    glBufferSubData(target, offset, size, data);
  }
}
