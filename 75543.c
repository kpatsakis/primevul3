void ExtensionService::LoadExtension(const FilePath& extension_path,
                                     bool prompt_for_plugins) {
  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
      NewRunnableMethod(backend_.get(),
                        &ExtensionServiceBackend::LoadSingleExtension,
                        extension_path, prompt_for_plugins));
}
