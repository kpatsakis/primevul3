  explicit BrowserClosedObserver(Browser* browser) {
    registrar_.Add(this, chrome::NOTIFICATION_BROWSER_CLOSED,
                   content::Source<Browser>(browser));
    content::RunMessageLoop();
  }
