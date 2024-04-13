void TabsDetectLanguageFunction::GotLanguage(const std::string& language) {
  SetResult(Value::CreateStringValue(language.c_str()));
  SendResponse(true);

  Release();  // Balanced in Run()
}
