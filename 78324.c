void RenderThreadImpl::AddEmbeddedWorkerRoute(int32 routing_id,
                                              IPC::Listener* listener) {
  AddRoute(routing_id, listener);
  if (devtools_agent_message_filter_.get()) {
    devtools_agent_message_filter_->AddEmbeddedWorkerRouteOnMainThread(
        routing_id);
  }
}
