ExtensionServiceBackend::~ExtensionServiceBackend() {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
        BrowserThread::CurrentlyOn(BrowserThread::FILE));
}
