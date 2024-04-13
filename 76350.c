bool VertexAttribManager::VertexAttribInfo::CanAccess(GLuint index) const {
  if (!enabled_) {
    return true;
  }

  if (!buffer_ || buffer_->IsDeleted()) {
    return false;
  }

  GLsizeiptr buffer_size = buffer_->size();
  if (offset_ > buffer_size || real_stride_ == 0) {
    return false;
  }

  uint32 usable_size = buffer_size - offset_;
  GLuint num_elements = usable_size / real_stride_ +
      ((usable_size % real_stride_) >=
       (GLES2Util::GetGLTypeSizeForTexturesAndBuffers(type_) * size_) ? 1 : 0);
  return index < num_elements;
}
