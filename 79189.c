void DevToolsWindow::Observe(int type,
                             const content::NotificationSource& source,
                             const content::NotificationDetails& details) {
  DCHECK_EQ(chrome::NOTIFICATION_BROWSER_THEME_CHANGED, type);
  UpdateTheme();
}
