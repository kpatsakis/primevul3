bool BeginInstallFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url()))
    return false;

  std::string id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetString(0, &id));
  if (!Extension::IdIsValid(id)) {
    error_ = kInvalidIdError;
    return false;
  }

  if (!user_gesture() && !ignore_user_gesture_for_tests) {
    error_ = kUserGestureRequiredError;
    return false;
  }

  CrxInstaller::SetWhitelistedInstallId(id);
  return true;
}
