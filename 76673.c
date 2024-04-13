void PpapiPluginProcessHost::OnChannelError() {
  DVLOG(1) << "PpapiPluginProcessHost" << (is_broker_ ? "[broker]" : "")
           << "::OnChannelError()";
  CancelRequests();
}
