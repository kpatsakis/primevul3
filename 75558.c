void ExtensionService::OnLoadedInstalledExtensions() {
  if (updater_.get()) {
    updater_->Start();
  }

  ready_ = true;
  NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSIONS_READY,
      Source<Profile>(profile_),
      NotificationService::NoDetails());
}
