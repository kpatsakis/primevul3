void ExtensionServiceBackend::CheckExtensionFileAccess(
    const FilePath& extension_path, bool prompt_for_plugins) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  std::string id = Extension::GenerateIdForPath(extension_path);
  bool allow_file_access =
      Extension::ShouldAlwaysAllowFileAccess(Extension::LOAD);
  if (frontend_->extension_prefs()->HasAllowFileAccessSetting(id))
    allow_file_access = frontend_->extension_prefs()->AllowFileAccess(id);

  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
      NewRunnableMethod(
          this,
          &ExtensionServiceBackend::LoadSingleExtensionWithFileAccess,
          extension_path, allow_file_access, prompt_for_plugins));
}
