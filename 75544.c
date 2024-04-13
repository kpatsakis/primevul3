void ExtensionService::LoadExtensionFromCommandLine(
    const FilePath& path_in) {

  base::ThreadRestrictions::ScopedAllowIO allow_io;

  FilePath extension_path = path_in;
  file_util::AbsolutePath(&extension_path);

  std::string id = Extension::GenerateIdForPath(extension_path);
  bool allow_file_access =
      Extension::ShouldAlwaysAllowFileAccess(Extension::LOAD);
  if (extension_prefs()->HasAllowFileAccessSetting(id))
    allow_file_access = extension_prefs()->AllowFileAccess(id);

  int flags = Extension::NO_FLAGS;
  if (allow_file_access)
    flags |= Extension::ALLOW_FILE_ACCESS;
  if (Extension::ShouldDoStrictErrorChecking(Extension::LOAD))
    flags |= Extension::STRICT_ERROR_CHECKS;

  std::string error;
  scoped_refptr<const Extension> extension(extension_file_util::LoadExtension(
      extension_path,
      Extension::LOAD,
      flags,
      &error));

  if (!extension) {
    ReportExtensionLoadError(
        extension_path,
        error,
        chrome::NOTIFICATION_EXTENSION_INSTALL_ERROR,
        true);
    return;
  }

  OnLoadSingleExtension(extension, false);
}
