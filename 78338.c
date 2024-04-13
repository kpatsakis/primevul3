RenderThreadImpl::GetFileThreadMessageLoopProxy() {
  DCHECK(message_loop() == base::MessageLoop::current());
  if (!file_thread_) {
    file_thread_.reset(new base::Thread("Renderer::FILE"));
    file_thread_->Start();
  }
  return file_thread_->message_loop_proxy();
}
