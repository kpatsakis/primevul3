void ExtensionService::SetBackgroundPageReady(const Extension* extension) {
  DCHECK(!extension->background_url().is_empty());
  extension_runtime_data_[extension->id()].background_page_ready = true;
  NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_BACKGROUND_PAGE_READY,
      Source<const Extension>(extension),
      NotificationService::NoDetails());
}
