  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) {
    if (type == NotificationType::NAV_LIST_PRUNED) {
      notification_count_++;
      details_ = *(Details<NavigationController::PrunedDetails>(details).ptr());
    }
  }
