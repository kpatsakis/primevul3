void ExecuteCodeInTabFunction::DidLoadAndLocalizeFile(bool success,
                                                      const std::string& data) {
  if (success) {
    if (!Execute(data))
      SendResponse(false);
  } else {
    error_ = ErrorUtils::FormatErrorMessage(keys::kLoadFileError,
        resource_.relative_path().AsUTF8Unsafe());
    SendResponse(false);
  }
}
