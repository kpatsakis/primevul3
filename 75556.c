void ExtensionServiceBackend::OnLoadSingleExtension(
    const scoped_refptr<const Extension>& extension, bool prompt_for_plugins) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (frontend_.get())
    frontend_->OnLoadSingleExtension(extension, prompt_for_plugins);
}
