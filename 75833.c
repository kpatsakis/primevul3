bool SetStoreLoginFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url()))
    return false;
  std::string login;
  EXTENSION_FUNCTION_VALIDATE(args_->GetString(0, &login));
  ExtensionService* service = profile_->GetExtensionService();
  ExtensionPrefs* prefs = service->extension_prefs();
  prefs->SetWebStoreLogin(login);
  return true;
}
