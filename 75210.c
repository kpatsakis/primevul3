void BrowserTitlebar::Observe(NotificationType type,
                              const NotificationSource& source,
                              const NotificationDetails& details) {
  switch (type.value) {
    case NotificationType::BROWSER_THEME_CHANGED:
      UpdateTextColor();
      break;

    default:
      NOTREACHED();
  }
}
