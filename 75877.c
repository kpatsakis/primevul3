  explicit PrunedListener(NavigationController* controller)
      : notification_count_(0) {
    registrar_.Add(this, NotificationType::NAV_LIST_PRUNED,
                   Source<NavigationController>(controller));
  }
