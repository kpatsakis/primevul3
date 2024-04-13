NotificationService::~NotificationService() {
  lazy_tls_ptr.Pointer()->Set(NULL);

#ifndef NDEBUG
  for (int i = 0; i < NotificationType::NOTIFICATION_TYPE_COUNT; i++) {
    if (observer_counts_[i] > 0) {
      LOG(INFO) << observer_counts_[i] << " notification observer(s) leaked"
          << " of notification type " << i;
    }
  }
#endif

  for (int i = 0; i < NotificationType::NOTIFICATION_TYPE_COUNT; i++) {
    NotificationSourceMap omap = observers_[i];
    for (NotificationSourceMap::iterator it = omap.begin();
         it != omap.end(); ++it) {
      delete it->second;
    }
  }
}
