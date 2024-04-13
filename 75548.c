scoped_refptr<CrxInstaller> ExtensionService::MakeCrxInstaller(
    ExtensionInstallUI* client) {
  return new CrxInstaller(weak_ptr_factory_.GetWeakPtr(), client);
}
