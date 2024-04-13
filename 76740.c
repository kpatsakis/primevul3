float GetDeviceScaleFactorFromMonitor(const aura::Window* window) {
  MonitorManager* monitor_manager = Env::GetInstance()->monitor_manager();
  return monitor_manager->GetMonitorNearestWindow(window).device_scale_factor();
}
