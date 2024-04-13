base::WaitableEvent* RenderThreadImpl::GetShutdownEvent() {
  return ChildProcess::current()->GetShutDownEvent();
}
