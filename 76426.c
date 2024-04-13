error::Error GLES2DecoderImpl::HandleGetShaderSource(
    uint32 immediate_data_size, const gles2::GetShaderSource& c) {
  GLuint shader = c.shader;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      shader, "glGetShaderSource");
  if (!info) {
    bucket->SetSize(0);
    return error::kNoError;
  }
  bucket->SetFromString(info->source());
  return error::kNoError;
}
