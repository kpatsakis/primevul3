void BeginInstallWithManifestFunction::OnParseFailure(
    ResultCode result_code, const std::string& error_message) {
  SetResult(result_code);
  error_ = error_message;
  SendResponse(false);

  Release();
}
