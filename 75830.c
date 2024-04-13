bool CompleteInstallFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url()))
    return false;

  std::string id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetString(0, &id));
  if (!Extension::IdIsValid(id)) {
    error_ = kInvalidIdError;
    return false;
  }

  if (!CrxInstaller::IsIdWhitelisted(id) &&
      !CrxInstaller::GetWhitelistedManifest(id)) {
    error_ = ExtensionErrorUtils::FormatErrorMessage(
        kNoPreviousBeginInstallError, id);
    return false;
  }

  std::vector<std::string> params;
  params.push_back("id=" + id);
  params.push_back("lang=" + g_browser_process->GetApplicationLocale());
  params.push_back("uc");
  std::string url_string = Extension::GalleryUpdateUrl(true).spec();

  GURL url(url_string + "?response=redirect&x=" +
      EscapeQueryParamValue(JoinString(params, '&'), true));
  DCHECK(url.is_valid());

  NavigationController& controller =
      dispatcher()->delegate()->associated_tab_contents()->controller();
  controller.LoadURL(url, source_url(), PageTransition::LINK);

  return true;
}
