PpapiPluginProcessHost::~PpapiPluginProcessHost() {
  DVLOG(1) << "PpapiPluginProcessHost" << (is_broker_ ? "[broker]" : "")
           << "~PpapiPluginProcessHost()";
  CancelRequests();
}
