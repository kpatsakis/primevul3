int32_t ImageDataPlatformBackend::GetSharedMemory(int* handle,
                                                   uint32_t* byte_count) {
  *handle = platform_image_->GetSharedMemoryHandle(byte_count);
  return PP_OK;
}
