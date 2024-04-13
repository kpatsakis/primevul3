void USBMountObserver::Observe(NotificationType type,
                               const NotificationSource& source,
                               const NotificationDetails& details) {
  DCHECK(type == NotificationType::BROWSER_CLOSED);
  for (BrowserIterator i = browsers_.begin(); i != browsers_.end();
       ++i) {
    if (Source<Browser>(source).ptr() == i->browser) {
      i->browser = NULL;
      registrar_.Remove(this,
                        NotificationType::BROWSER_CLOSED,
                        source);
      return;
    }
  }
}
