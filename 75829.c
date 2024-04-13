bool BeginInstallWithManifestFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url())) {
    SetResult(PERMISSION_DENIED);
    return false;
  }

  if (!user_gesture() && !ignore_user_gesture_for_tests) {
    SetResult(NO_GESTURE);
    error_ = kUserGestureRequiredError;
    return false;
  }

  EXTENSION_FUNCTION_VALIDATE(args_->GetString(0, &id_));
  if (!Extension::IdIsValid(id_)) {
    SetResult(INVALID_ID);
    error_ = kInvalidIdError;
    return false;
  }

  EXTENSION_FUNCTION_VALIDATE(args_->GetString(1, &icon_data_));
  EXTENSION_FUNCTION_VALIDATE(args_->GetString(2, &manifest_));

  scoped_refptr<SafeBeginInstallHelper> helper =
      new SafeBeginInstallHelper(this, icon_data_, manifest_);
  helper->Start();

  AddRef();

  return true;
}
