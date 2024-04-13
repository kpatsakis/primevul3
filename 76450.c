    VertexAttribInfo()
        : index_(0),
          enabled_(false),
          size_(4),
          type_(GL_FLOAT),
          offset_(0),
          normalized_(GL_FALSE),
          gl_stride_(0),
          real_stride_(16),
          list_(NULL) {
      value_.v[0] = 0.0f;
      value_.v[1] = 0.0f;
      value_.v[2] = 0.0f;
      value_.v[3] = 1.0f;
    }
