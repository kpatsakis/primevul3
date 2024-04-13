bool TabsDetectLanguageFunction::RunImpl() {
  int tab_id = 0;
  Browser* browser = NULL;
  WebContents* contents = NULL;

  if (HasOptionalArgument(0)) {
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));
    if (!GetTabById(tab_id, profile(), include_incognito(),
                    &browser, NULL, &contents, NULL, &error_)) {
      return false;
    }
    if (!browser || !contents)
      return false;
  } else {
    browser = GetCurrentBrowser();
    if (!browser)
      return false;
    contents = browser->tab_strip_model()->GetActiveWebContents();
    if (!contents)
      return false;
  }

  if (contents->GetController().NeedsReload()) {
    error_ = keys::kCannotDetermineLanguageOfUnloadedTab;
    return false;
  }

  AddRef();  // Balanced in GotLanguage().

  TranslateTabHelper* translate_tab_helper =
      TranslateTabHelper::FromWebContents(contents);
  if (!translate_tab_helper->language_state().original_language().empty()) {
    MessageLoop::current()->PostTask(FROM_HERE, base::Bind(
        &TabsDetectLanguageFunction::GotLanguage, this,
        translate_tab_helper->language_state().original_language()));
    return true;
  }
  registrar_.Add(this, chrome::NOTIFICATION_TAB_LANGUAGE_DETERMINED,
                 content::Source<WebContents>(contents));
  registrar_.Add(
      this, chrome::NOTIFICATION_TAB_CLOSING,
      content::Source<NavigationController>(&(contents->GetController())));
  registrar_.Add(
      this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      content::Source<NavigationController>(&(contents->GetController())));
  return true;
}
