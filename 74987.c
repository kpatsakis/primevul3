USBMountObserver::BrowserIterator USBMountObserver::FindBrowserForPath(
    const std::string& path) {
  for (BrowserIterator i = browsers_.begin();i != browsers_.end();
       ++i) {
    if (path.find(i->device_path) != std::string::npos) {
      return i;
    }
  }
  return browsers_.end();
}
