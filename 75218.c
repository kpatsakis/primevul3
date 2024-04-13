void BrowserTitlebar::ShowFaviconMenu(GdkEventButton* event) {
  if (!favicon_menu_.get()) {
    favicon_menu_.reset(new MenuGtk(this,
        GetFaviconMenu(browser_window_->browser()->profile(), this), NULL));
  }

  favicon_menu_->Popup(app_mode_favicon_, reinterpret_cast<GdkEvent*>(event));
}
