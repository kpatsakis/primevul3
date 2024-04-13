error::Error GLES2DecoderImpl::HandleCommandBufferEnable(
    uint32 immediate_data_size, const gles2::CommandBufferEnable& c) {
  Bucket* bucket = GetBucket(c.bucket_id);
  typedef gles2::CommandBufferEnable::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (*result != 0) {
    return error::kInvalidArguments;
  }
  std::string feature_str;
  if (!bucket->GetAsString(&feature_str)) {
    return error::kInvalidArguments;
  }

  if (feature_str.compare(PEPPER3D_ALLOW_BUFFERS_ON_MULTIPLE_TARGETS) == 0) {
    buffer_manager()->set_allow_buffers_on_multiple_targets(true);
  } else if (feature_str.compare(PEPPER3D_SKIP_GLSL_TRANSLATION) == 0) {
    use_shader_translator_ = false;
  } else {
    return error::kNoError;
  }

  *result = 1;  // true.
  return error::kNoError;
}
