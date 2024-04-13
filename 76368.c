void GLES2DecoderImpl::DoBufferData(
    GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage) {
  if (!validators_->buffer_target.IsValid(target)) {
    SetGLError(GL_INVALID_ENUM, "glBufferData: target GL_INVALID_ENUM");
    return;
  }
  if (!validators_->buffer_usage.IsValid(usage)) {
    SetGLError(GL_INVALID_ENUM, "glBufferData: usage GL_INVALID_ENUM");
    return;
  }
  if (size < 0) {
    SetGLError(GL_INVALID_VALUE, "glBufferData: size < 0");
    return;
  }
  BufferManager::BufferInfo* info = GetBufferInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glBufferData: unknown buffer");
    return;
  }
  scoped_array<int8> zero;
  if (!data) {
    zero.reset(new int8[size]);
    memset(zero.get(), 0, size);
    data = zero.get();
  }
  CopyRealGLErrorsToWrapper();
  glBufferData(target, size, data, usage);
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    SetGLError(error, NULL);
  } else {
    buffer_manager()->SetSize(info, size);
    info->SetRange(0, size, data);
  }
}
