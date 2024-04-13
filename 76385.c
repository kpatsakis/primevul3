GLuint GLES2DecoderImpl::DoGetMaxValueInBuffer(
    GLuint buffer_id, GLsizei count, GLenum type, GLuint offset) {
  GLuint max_vertex_accessed = 0;
  BufferManager::BufferInfo* info = GetBufferInfo(buffer_id);
  if (!info) {
    SetGLError(GL_INVALID_VALUE,
               "GetMaxValueInBuffer: unknown buffer");
  } else {
    if (!info->GetMaxValueForRange(offset, count, type, &max_vertex_accessed)) {
      SetGLError(GL_INVALID_OPERATION,
                 "GetMaxValueInBuffer: range out of bounds for buffer");
    }
  }
  return max_vertex_accessed;
}
