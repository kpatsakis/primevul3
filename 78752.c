void DelegatedFrameHost::ReturnResources(
    const cc::ReturnedResourceArray& resources) {
  if (resources.empty())
    return;
  std::copy(resources.begin(),
            resources.end(),
            std::back_inserter(surface_returned_resources_));
  if (!pending_delegated_ack_count_)
    SendReturnedDelegatedResources(last_output_surface_id_);
}
