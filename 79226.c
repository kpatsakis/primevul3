Browser* CreateBrowserWindow(const Browser::CreateParams& params,
                             Profile* profile,
                             const std::string& extension_id) {
  bool use_existing_browser_window = false;

#if defined(OS_WIN)
  if (win8::IsSingleWindowMetroMode())
    use_existing_browser_window = true;
#endif  // OS_WIN

  Browser* new_window = NULL;
  if (use_existing_browser_window)
    new_window = chrome::FindTabbedBrowser(profile, false,
                                           params.host_desktop_type);

  if (!new_window)
    new_window = new Browser(params);
  return new_window;
}
