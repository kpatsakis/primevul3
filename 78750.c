void DelegatedFrameHostClient::RequestCopyOfOutput(
    scoped_ptr<cc::CopyOutputRequest> request) {
  GetDelegatedFrameHost()->RequestCopyOfOutput(request.Pass());
}
