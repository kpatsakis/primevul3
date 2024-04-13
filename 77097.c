void GpuProcessHostUIShim::OnGraphicsInfoCollected(
    const content::GPUInfo& gpu_info) {
  TRACE_EVENT0("test_gpu", "OnGraphicsInfoCollected");

  GpuDataManagerImpl::GetInstance()->UpdateGpuInfo(gpu_info);
}
