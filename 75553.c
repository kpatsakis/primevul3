void ExtensionService::OnExternalExtensionFileFound(
         const std::string& id,
         const Version* version,
         const FilePath& path,
         Extension::Location location) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  CHECK(Extension::IdIsValid(id));
  if (extension_prefs_->IsExternalExtensionUninstalled(id))
    return;

  DCHECK(version);

  const Extension* existing = GetExtensionById(id, true);
  if (existing) {
    switch (existing->version()->CompareTo(*version)) {
      case -1:  // existing version is older, we should upgrade
        break;
      case 0:  // existing version is same, do nothing
        return;
      case 1:  // existing version is newer, uh-oh
        LOG(WARNING) << "Found external version of extension " << id
                     << "that is older than current version. Current version "
                     << "is: " << existing->VersionString() << ". New version "
                     << "is: " << version << ". Keeping current version.";
        return;
    }
  }

  pending_extension_manager()->AddFromExternalFile(id, location);

  scoped_refptr<CrxInstaller> installer(MakeCrxInstaller(NULL));
  installer->set_install_source(location);
  installer->set_expected_id(id);
  installer->set_expected_version(*version);
  installer->set_install_cause(extension_misc::INSTALL_CAUSE_EXTERNAL_FILE);
  installer->InstallCrx(path);
}
