void ExtensionPrefs::SetAppLauncherOrder(
    const std::vector<std::string>& extension_ids) {
  for (size_t i = 0; i < extension_ids.size(); ++i)
    SetAppLaunchIndex(extension_ids.at(i), i);

  NotificationService::current()->Notify(
      NotificationType::EXTENSION_LAUNCHER_REORDERED,
      Source<ExtensionPrefs>(this),
      NotificationService::NoDetails());
}
