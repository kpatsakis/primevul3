bool GetBrowserLoginFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url()))
    return false;
  result_.reset(CreateLoginResult(GetDefaultProfile(profile_)));
  return true;
}
