void ExecuteCodeInTabFunction::LocalizeCSS(
    const std::string& data,
    const std::string& extension_id,
    const base::FilePath& extension_path,
    const std::string& extension_default_locale) {
  scoped_ptr<SubstitutionMap> localization_messages(
      extension_file_util::LoadMessageBundleSubstitutionMap(
          extension_path, extension_id, extension_default_locale));

  std::string css_data = data;
  std::string error;
  extensions::MessageBundle::ReplaceMessagesWithExternalDictionary(
      *localization_messages, &css_data, &error);

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&ExecuteCodeInTabFunction::DidLoadAndLocalizeFile, this,
                 true, css_data));
}
