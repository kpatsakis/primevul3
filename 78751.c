void DelegatedFrameHost::RequestCopyOfOutput(
    scoped_ptr<cc::CopyOutputRequest> request) {
  client_->GetLayer()->RequestCopyOfOutput(request.Pass());
}
