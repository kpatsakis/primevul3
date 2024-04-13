int32_t PPB_ImageData_Impl::GetSharedMemory(int* handle, uint32_t* byte_count) {
  return backend_->GetSharedMemory(handle, byte_count);
}
