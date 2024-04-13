void RenderThreadImpl::SetGamepadListener(blink::WebGamepadListener* listener) {
  gamepad_shared_memory_reader_->SetGamepadListener(listener);
}
