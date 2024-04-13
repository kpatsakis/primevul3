void RenderThreadImpl::RemoveRoute(int32 routing_id) {
  ChildThread::GetRouter()->RemoveRoute(routing_id);
}
