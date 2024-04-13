void RenderThreadImpl::SampleGamepads(blink::WebGamepads* data) {
  gamepad_shared_memory_reader_->SampleGamepads(*data);
}
