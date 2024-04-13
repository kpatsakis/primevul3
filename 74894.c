WebKit::WebDeviceOrientationClient* RenderView::deviceOrientationClient() {
  if (!device_orientation_dispatcher_.get())
    device_orientation_dispatcher_.reset(new DeviceOrientationDispatcher(this));
  return device_orientation_dispatcher_.get();
}
