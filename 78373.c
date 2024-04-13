void RenderThreadImpl::RemoveFilter(IPC::MessageFilter* filter) {
  channel()->RemoveFilter(filter);
}
