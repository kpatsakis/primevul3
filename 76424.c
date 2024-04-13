error::Error GLES2DecoderImpl::HandleGetProgramInfoLog(
    uint32 immediate_data_size, const gles2::GetProgramInfoLog& c) {
  GLuint program = c.program;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glGetProgramInfoLog");
  if (!info) {
    return error::kNoError;
  }
  bucket->SetFromString(info->log_info());
  return error::kNoError;
}
