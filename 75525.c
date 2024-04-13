void ExtensionService::Init() {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DCHECK(!ready_);  // Can't redo init.
  DCHECK_EQ(extensions_.size(), 0u);

  g_browser_process->resource_dispatcher_host();

  LoadAllExtensions();

  CheckForExternalUpdates();

  GarbageCollectExtensions();
}
