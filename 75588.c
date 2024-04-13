bool ExtensionService::UpdateExtension(
    const std::string& id,
    const FilePath& extension_path,
    const GURL& download_url,
    CrxInstaller** out_crx_installer) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  PendingExtensionInfo pending_extension_info;
  bool is_pending_extension = pending_extension_manager_.GetById(
      id, &pending_extension_info);

  const Extension* extension =
      GetExtensionByIdInternal(id, true, true, false);
  if (!is_pending_extension && !extension) {
    LOG(WARNING) << "Will not update extension " << id
                 << " because it is not installed or pending";
    if (!BrowserThread::PostTask(
            BrowserThread::FILE, FROM_HERE,
            NewRunnableFunction(
                extension_file_util::DeleteFile, extension_path, false)))
      NOTREACHED();

    return false;
  }

  ExtensionInstallUI* client =
      (!is_pending_extension || pending_extension_info.install_silently()) ?
      NULL : new ExtensionInstallUI(profile_);

  scoped_refptr<CrxInstaller> installer(MakeCrxInstaller(client));
  installer->set_expected_id(id);
  if (is_pending_extension)
    installer->set_install_source(pending_extension_info.install_source());
  else if (extension)
    installer->set_install_source(extension->location());
  if (pending_extension_info.install_silently())
    installer->set_allow_silent_install(true);
  installer->set_delete_source(true);
  installer->set_original_url(download_url);
  installer->set_install_cause(extension_misc::INSTALL_CAUSE_UPDATE);
  installer->InstallCrx(extension_path);

  if (out_crx_installer)
    *out_crx_installer = installer;

  return true;
}
