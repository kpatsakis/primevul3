void ExtensionServiceBackend::LoadSingleExtension(const FilePath& path_in,
                                                  bool prompt_for_plugins) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));

  FilePath extension_path = path_in;
  file_util::AbsolutePath(&extension_path);

  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
      NewRunnableMethod(this,
                        &ExtensionServiceBackend::CheckExtensionFileAccess,
                        extension_path, prompt_for_plugins));
}
