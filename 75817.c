void BeginInstallWithManifestFunction::InstallUIProceed() {
  CrxInstaller::SetWhitelistedManifest(id_, parsed_manifest_.release());
  SetResult(ERROR_NONE);
  SendResponse(true);

  Release();
}
