void DelegatedFrameHost::UnusedResourcesAreAvailable() {
  if (pending_delegated_ack_count_)
    return;

  SendReturnedDelegatedResources(last_output_surface_id_);
}
