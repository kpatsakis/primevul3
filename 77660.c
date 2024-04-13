error::Error GLES2DecoderImpl::HandleGetTranslatedShaderSourceANGLE(
    uint32 immediate_data_size,
    const gles2::GetTranslatedShaderSourceANGLE& c) {
  GLuint shader = c.shader;

  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      shader, "glTranslatedGetShaderSourceANGLE");
  if (!info) {
    bucket->SetSize(0);
    return error::kNoError;
  }

  bucket->SetFromString(info->translated_source() ?
      info->translated_source()->c_str() : NULL);
  return error::kNoError;
}
