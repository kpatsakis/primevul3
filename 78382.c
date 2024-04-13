void RenderThreadImpl::SetResourceDispatcherDelegate(
    ResourceDispatcherDelegate* delegate) {
  resource_dispatcher()->set_delegate(delegate);
}
