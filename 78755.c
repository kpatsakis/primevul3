void DelegatedFrameHost::SendDelegatedFrameAck(uint32 output_surface_id) {
  RenderWidgetHostImpl* host = client_->GetHost();
  cc::CompositorFrameAck ack;
  if (!surface_returned_resources_.empty())
    ack.resources.swap(surface_returned_resources_);
  if (resource_collection_)
    resource_collection_->TakeUnusedResourcesForChildCompositor(&ack.resources);
  RenderWidgetHostImpl::SendSwapCompositorFrameAck(host->GetRoutingID(),
                                                   output_surface_id,
                                                   host->GetProcess()->GetID(),
                                                   ack);
  DCHECK_GT(pending_delegated_ack_count_, 0);
  pending_delegated_ack_count_--;
}
