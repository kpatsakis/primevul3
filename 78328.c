void RenderThreadImpl::AddRoute(int32 routing_id, IPC::Listener* listener) {
  ChildThread::GetRouter()->AddRoute(routing_id, listener);
  std::map<int, mojo::MessagePipeHandle>::iterator it =
      pending_render_frame_connects_.find(routing_id);
  if (it == pending_render_frame_connects_.end())
    return;

  RenderFrameImpl* frame = RenderFrameImpl::FromRoutingID(routing_id);
  if (!frame)
    return;

  mojo::ScopedMessagePipeHandle handle(it->second);
  pending_render_frame_connects_.erase(it);
  frame->BindServiceRegistry(handle.Pass());
}
