void ExecuteCodeInTabFunction::DidLoadFile(bool success,
                                           const std::string& data) {
  std::string function_name = name();
  const extensions::Extension* extension = GetExtension();

  if (success &&
      function_name == TabsInsertCSSFunction::function_name() &&
      extension != NULL &&
      data.find(
          extensions::MessageBundle::kMessageBegin) != std::string::npos) {
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&ExecuteCodeInTabFunction::LocalizeCSS, this,
                   data,
                   extension->id(),
                   extension->path(),
                   extensions::LocaleInfo::GetDefaultLocale(extension)));
  } else {
    DidLoadAndLocalizeFile(success, data);
  }
}
