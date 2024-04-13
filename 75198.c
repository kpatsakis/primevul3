BrowserTitlebar::BrowserTitlebar(BrowserWindowGtk* browser_window,
                                 GtkWindow* window)
    : browser_window_(browser_window), window_(window),
      app_mode_favicon_(NULL),
      app_mode_title_(NULL),
      using_custom_frame_(false),
      window_has_focus_(false),
      theme_provider_(NULL) {
  Init();
}
