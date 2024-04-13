void RegisterForAllNavNotifications(TestNotificationTracker* tracker,
                                    NavigationController* controller) {
  tracker->ListenFor(NotificationType::NAV_ENTRY_COMMITTED,
                     Source<NavigationController>(controller));
  tracker->ListenFor(NotificationType::NAV_LIST_PRUNED,
                     Source<NavigationController>(controller));
  tracker->ListenFor(NotificationType::NAV_ENTRY_CHANGED,
                      Source<NavigationController>(controller));
 }
