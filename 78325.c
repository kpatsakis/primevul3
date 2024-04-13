void RenderThreadImpl::AddFilter(IPC::MessageFilter* filter) {
  channel()->AddFilter(filter);
}
