    RenderThreadImpl::GetIOMessageLoopProxy() {
  return ChildProcess::current()->io_message_loop_proxy();
}
