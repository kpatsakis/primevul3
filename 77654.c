error::Error GLES2DecoderImpl::HandleDestroyStreamTextureCHROMIUM(
    uint32 immediate_data_size,
    const gles2::DestroyStreamTextureCHROMIUM& c) {
  GLuint client_id = c.texture;
  TextureManager::TextureInfo* info =
      texture_manager()->GetTextureInfo(client_id);
  if (info && info->IsStreamTexture()) {
    if (!stream_texture_manager_)
      return error::kInvalidArguments;

    stream_texture_manager_->DestroyStreamTexture(info->service_id());
    info->SetStreamTexture(false);
    texture_manager()->SetInfoTarget(info, 0);
  } else {
    SetGLError(GL_INVALID_VALUE,
               "glDestroyStreamTextureCHROMIUM: bad texture id.");
  }

  return error::kNoError;
}
