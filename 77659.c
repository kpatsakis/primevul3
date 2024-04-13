error::Error GLES2DecoderImpl::HandleGetString(
    uint32 immediate_data_size, const gles2::GetString& c) {
  GLenum name = static_cast<GLenum>(c.name);
  if (!validators_->string_type.IsValid(name)) {
    SetGLError(GL_INVALID_ENUM, "glGetString: name GL_INVALID_ENUM");
    return error::kNoError;
  }
  const char* gl_str = reinterpret_cast<const char*>(glGetString(name));
  const char* str = NULL;
  std::string extensions;
  switch (name) {
    case GL_VERSION:
      str = "OpenGL ES 2.0 Chromium";
      break;
    case GL_SHADING_LANGUAGE_VERSION:
      str = "OpenGL ES GLSL ES 1.0 Chromium";
      break;
    case GL_EXTENSIONS:
      {
        if (force_webgl_glsl_validation_ &&
            !derivatives_explicitly_enabled_) {
          extensions = feature_info_->extensions();
          size_t offset = extensions.find(kOESDerivativeExtension);
          if (std::string::npos != offset) {
            extensions.replace(offset,
                               offset + arraysize(kOESDerivativeExtension),
                               std::string());
          }
          str = extensions.c_str();
        } else {
          str = feature_info_->extensions().c_str();
        }
      }
      break;
    default:
      str = gl_str;
      break;
  }
  Bucket* bucket = CreateBucket(c.bucket_id);
  bucket->SetFromString(str);
  return error::kNoError;
}
