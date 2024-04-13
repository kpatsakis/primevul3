void NotificationService::Notify(NotificationType type,
                                 const NotificationSource& source,
                                 const NotificationDetails& details) {
  DCHECK(type.value > NotificationType::ALL) <<
      "Allowed for observing, but not posting.";
  DCHECK(type.value < NotificationType::NOTIFICATION_TYPE_COUNT);


  if (HasKey(observers_[NotificationType::ALL], AllSources()) &&
      source != AllSources()) {
    FOR_EACH_OBSERVER(NotificationObserver,
       *observers_[NotificationType::ALL][AllSources().map_key()],
       Observe(type, source, details));
  }

  if (HasKey(observers_[NotificationType::ALL], source)) {
    FOR_EACH_OBSERVER(NotificationObserver,
        *observers_[NotificationType::ALL][source.map_key()],
        Observe(type, source, details));
  }

  if (HasKey(observers_[type.value], AllSources()) &&
      source != AllSources()) {
    FOR_EACH_OBSERVER(NotificationObserver,
                      *observers_[type.value][AllSources().map_key()],
                      Observe(type, source, details));
  }

  if (HasKey(observers_[type.value], source)) {
    FOR_EACH_OBSERVER(NotificationObserver,
                      *observers_[type.value][source.map_key()],
                      Observe(type, source, details));
  }
}
