void BeginInstallWithManifestFunction::InstallUIAbort() {
  error_ = std::string(kUserCancelledError);
  SetResult(USER_CANCELLED);
  SendResponse(false);

  Release();
}
