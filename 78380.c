void RenderThreadImpl::HistogramCustomizer::SetCommonHost(
    const std::string& host) {
  if (host != common_host_) {
    common_host_ = host;
    common_host_histogram_suffix_ = HostToCustomHistogramSuffix(host);
    blink::mainThreadIsolate()->SetCreateHistogramFunction(CreateHistogram);
  }
}
