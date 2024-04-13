PpapiPluginProcessHost::PpapiPluginProcessHost()
    : is_broker_(true) {
  process_.reset(new BrowserChildProcessHostImpl(
      content::PROCESS_TYPE_PPAPI_BROKER, this));

  ppapi::PpapiPermissions permissions;  // No permissions.
  host_impl_ = new content::BrowserPpapiHostImpl(this, permissions);
}
