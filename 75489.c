void ExtensionService::CheckForUpdatesSoon() {
  if (updater()) {
    updater()->CheckSoon();
  } else {
    LOG(WARNING) << "CheckForUpdatesSoon() called with auto-update turned off";
  }
}
