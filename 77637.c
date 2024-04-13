void GLES2DecoderImpl::DoTexSubImage2D(
  GLenum target,
  GLint level,
  GLint xoffset,
  GLint yoffset,
  GLsizei width,
  GLsizei height,
  GLenum format,
  GLenum type,
  const void * data) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexSubImage2D: unknown texture for target");
    return;
  }
  GLenum current_type = 0;
  GLenum internal_format = 0;
  if (!info->GetLevelType(target, level, &current_type, &internal_format)) {
    SetGLError(
        GL_INVALID_OPERATION,
        "glTexSubImage2D: level does not exist.");
    return;
  }
  if (format != internal_format) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexSubImage2D: format does not match internal format.");
    return;
  }
  if (type != current_type) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexSubImage2D: type does not match type of texture.");
    return;
  }

  if (!info->ValidForTexture(
          target, level, xoffset, yoffset, width, height, format, type)) {
    SetGLError(GL_INVALID_VALUE,
               "glTexSubImage2D: bad dimensions.");
    return;
  }

  GLsizei tex_width = 0;
  GLsizei tex_height = 0;
  bool ok = info->GetLevelSize(target, level, &tex_width, &tex_height);
  DCHECK(ok);
  if (xoffset != 0 || yoffset != 0 ||
      width != tex_width || height != tex_height) {
    if (!texture_manager()->ClearTextureLevel(this, info, target, level)) {
      SetGLError(GL_OUT_OF_MEMORY, "glTexSubImage2D: dimensions too big");
      return;
    }
    glTexSubImage2D(
        target, level, xoffset, yoffset, width, height, format, type, data);
    return;
  }

  if (teximage2d_faster_than_texsubimage2d_ && !info->IsImmutable()) {
    WrappedTexImage2D(
        target, level, format, width, height, 0, format, type, data);
  } else {
    glTexSubImage2D(
        target, level, xoffset, yoffset, width, height, format, type, data);
  }
  texture_manager()->SetLevelCleared(info, target, level);
}
