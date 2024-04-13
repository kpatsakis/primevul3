Network::Network(const ServiceInfo* service) {
  type_ = service->type;
  state_ = service->state;
  error_ = service->error;
  service_path_ = SafeString(service->service_path);
  device_path_ = SafeString(service->device_path);
  is_active_ = service->is_active;
  ip_address_.clear();
  if (EnsureCrosLoaded() && connected() && service->device_path) {
    IPConfigStatus* ipconfig_status = ListIPConfigs(service->device_path);
    if (ipconfig_status) {
      for (int i = 0; i < ipconfig_status->size; i++) {
        IPConfig ipconfig = ipconfig_status->ips[i];
        if (strlen(ipconfig.address) > 0)
          ip_address_ = ipconfig.address;
      }
      FreeIPConfigStatus(ipconfig_status);
    }
  }
}
