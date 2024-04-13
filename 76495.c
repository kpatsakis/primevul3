void SandboxedExtensionUnpacker::OnUnpackExtensionSucceeded(
    const DictionaryValue& manifest) {
  if (thread_identifier_ != BrowserThread::ID_COUNT)
    DCHECK(BrowserThread::CurrentlyOn(thread_identifier_));
  got_response_ = true;

  scoped_ptr<DictionaryValue> final_manifest(RewriteManifestFile(manifest));
  if (!final_manifest.get())
    return;


  std::string error;
  if (!extension_l10n_util::LocalizeExtension(extension_root_,
                                              final_manifest.get(),
                                              &error)) {
    ReportFailure(error);
    return;
  }

  extension_ = Extension::Create(
      extension_root_, Extension::INTERNAL, *final_manifest, true, &error);

  if (!extension_.get()) {
    ReportFailure(std::string("Manifest is invalid: ") + error);
    return;
  }

  if (!RewriteImageFiles())
    return;

  if (!RewriteCatalogFiles())
    return;

  ReportSuccess();
}
