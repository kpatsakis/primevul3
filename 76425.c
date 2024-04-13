error::Error GLES2DecoderImpl::HandleGetShaderInfoLog(
    uint32 immediate_data_size, const gles2::GetShaderInfoLog& c) {
  GLuint shader = c.shader;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      shader, "glGetShaderInfoLog");
  if (!info) {
    bucket->SetSize(0);
    return error::kNoError;
  }
  bucket->SetFromString(info->log_info());
  return error::kNoError;
}
