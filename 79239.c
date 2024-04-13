void TabsDetectLanguageFunction::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  std::string language;
  if (type == chrome::NOTIFICATION_TAB_LANGUAGE_DETERMINED)
    language = *content::Details<std::string>(details).ptr();

  registrar_.RemoveAll();

  GotLanguage(language);
}
