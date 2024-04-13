void ExtensionServiceBackend::LoadSingleExtensionWithFileAccess(
    const FilePath& extension_path,
    bool allow_file_access,
    bool prompt_for_plugins) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  int flags = allow_file_access ?
      Extension::ALLOW_FILE_ACCESS : Extension::NO_FLAGS;
  if (Extension::ShouldDoStrictErrorChecking(Extension::LOAD))
    flags |= Extension::STRICT_ERROR_CHECKS;
  std::string error;
  scoped_refptr<const Extension> extension(extension_file_util::LoadExtension(
      extension_path,
      Extension::LOAD,
      flags,
      &error));

  if (!extension) {
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
        NewRunnableMethod(
            this,
            &ExtensionServiceBackend::ReportExtensionLoadError,
            extension_path, error));
    return;
  }

  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
      NewRunnableMethod(
          this,
          &ExtensionServiceBackend::OnLoadSingleExtension,
          extension, prompt_for_plugins));
}
