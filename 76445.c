    void SetInfo(
        BufferManager::BufferInfo* buffer,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei gl_stride,
        GLsizei real_stride,
        GLsizei offset) {
      DCHECK_GT(real_stride, 0);
      buffer_ = buffer;
      size_ = size;
      type_ = type;
      normalized_ = normalized;
      gl_stride_ = gl_stride;
      real_stride_ = real_stride;
      offset_ = offset;
    }
