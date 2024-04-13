void RenderThreadImpl::RegisterPendingRenderFrameConnect(
    int routing_id,
    mojo::ScopedMessagePipeHandle handle) {
  std::pair<std::map<int, mojo::MessagePipeHandle>::iterator, bool> result =
      pending_render_frame_connects_.insert(
          std::make_pair(routing_id, handle.release()));
  CHECK(result.second) << "Inserting a duplicate item.";
}
