error::Error GLES2DecoderImpl::HandleCreateStreamTextureCHROMIUM(
    uint32 immediate_data_size,
    const gles2::CreateStreamTextureCHROMIUM& c) {
  if (!feature_info_->feature_flags().chromium_stream_texture) {
    SetGLError(GL_INVALID_OPERATION,
               "glOpenStreamTextureCHROMIUM: "
               "not supported.");
    return error::kNoError;
  }

  uint32 client_id = c.client_id;
  typedef gles2::CreateStreamTextureCHROMIUM::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));

  if (!result)
    return error::kOutOfBounds;
  *result = GL_ZERO;
  TextureManager::TextureInfo* info =
      texture_manager()->GetTextureInfo(client_id);
  if (!info) {
    SetGLError(GL_INVALID_VALUE,
               "glCreateStreamTextureCHROMIUM: "
               "bad texture id.");
    return error::kNoError;
  }

  if (info->IsStreamTexture()) {
    SetGLError(GL_INVALID_OPERATION,
               "glCreateStreamTextureCHROMIUM: "
               "is already a stream texture.");
    return error::kNoError;
  }

  if (info->target() && info->target() != GL_TEXTURE_EXTERNAL_OES) {
    SetGLError(GL_INVALID_OPERATION,
               "glCreateStreamTextureCHROMIUM: "
               "is already bound to incompatible target.");
    return error::kNoError;
  }

  if (!stream_texture_manager_)
    return error::kInvalidArguments;

  GLuint object_id = stream_texture_manager_->CreateStreamTexture(
      info->service_id(), client_id);

  if (object_id) {
    info->SetStreamTexture(true);
  } else {
    SetGLError(GL_OUT_OF_MEMORY,
               "glCreateStreamTextureCHROMIUM: "
               "failed to create platform texture.");
  }

  *result = object_id;
  return error::kNoError;
}
