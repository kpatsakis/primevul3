BrowserTitlebar::~BrowserTitlebar() {
  ActiveWindowWatcherX::RemoveObserver(this);
}
