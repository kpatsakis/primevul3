void ExtensionService::ReportExtensionLoadError(
    const FilePath& extension_path,
    const std::string &error,
    int type,
    bool be_noisy) {
  NotificationService* service = NotificationService::current();
  service->Notify(type,
                  Source<Profile>(profile_),
                  Details<const std::string>(&error));

  std::string path_str = UTF16ToUTF8(extension_path.LossyDisplayName());
  std::string message = base::StringPrintf(
      "Could not load extension from '%s'. %s",
      path_str.c_str(), error.c_str());
  ExtensionErrorReporter::GetInstance()->ReportError(message, be_noisy);
}
