bool GetStoreLoginFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url()))
    return false;
  ExtensionService* service = profile_->GetExtensionService();
  ExtensionPrefs* prefs = service->extension_prefs();
  std::string login;
  if (prefs->GetWebStoreLogin(&login)) {
    result_.reset(Value::CreateStringValue(login));
  } else {
    result_.reset(Value::CreateStringValue(std::string()));
  }
  return true;
}
