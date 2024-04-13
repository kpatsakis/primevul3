AutoFillManager::~AutoFillManager() {
  download_manager_.SetObserver(NULL);
}
