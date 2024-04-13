error::Error GLES2DecoderImpl::DoTexImage2D(
  GLenum target,
  GLint level,
  GLenum internal_format,
  GLsizei width,
  GLsizei height,
  GLint border,
  GLenum format,
  GLenum type,
  const void* pixels,
  uint32 pixels_size) {
  if (!validators_->texture_target.IsValid(target)) {
    SetGLError(GL_INVALID_ENUM, "glTexImage2D: target GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->texture_format.IsValid(internal_format)) {
    SetGLError(GL_INVALID_ENUM,
               "glTexImage2D: internal_format GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->texture_format.IsValid(format)) {
    SetGLError(GL_INVALID_ENUM, "glTexImage2D: format GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->pixel_type.IsValid(type)) {
    SetGLError(GL_INVALID_ENUM, "glTexImage2D: type GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (format != internal_format) {
    SetGLError(GL_INVALID_OPERATION, "glTexImage2D: format != internalFormat");
    return error::kNoError;
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, 1) ||
      border != 0) {
    SetGLError(GL_INVALID_VALUE, "glTexImage2D: dimensions out of range");
    return error::kNoError;
  }
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImage2D: unknown texture for target");
    return error::kNoError;
  }
  scoped_array<int8> zero;
  if (!pixels) {
    zero.reset(new int8[pixels_size]);
    memset(zero.get(), 0, pixels_size);
    pixels = zero.get();
  }

  GLenum gl_internal_format = internal_format;
  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    if (format == GL_BGRA_EXT && internal_format == GL_BGRA_EXT) {
      gl_internal_format = GL_RGBA;
    } else if (type == GL_FLOAT) {
      if (format == GL_RGBA) {
        gl_internal_format = GL_RGBA32F_ARB;
      } else if (format == GL_RGB) {
        gl_internal_format = GL_RGB32F_ARB;
      }
    } else if (type == GL_HALF_FLOAT_OES) {
      if (format == GL_RGBA) {
        gl_internal_format = GL_RGBA16F_ARB;
      } else if (format == GL_RGB) {
        gl_internal_format = GL_RGB16F_ARB;
      }
    }
  }

  CopyRealGLErrorsToWrapper();
  glTexImage2D(
      target, level, gl_internal_format, width, height, border, format, type,
      pixels);
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(info,
        target, level, internal_format, width, height, 1, border, format, type);
  }
  return error::kNoError;
}
