void NotificationService::AddObserver(NotificationObserver* observer,
                                      NotificationType type,
                                      const NotificationSource& source) {
  DCHECK(type.value < NotificationType::NOTIFICATION_TYPE_COUNT);

  CHECK(observer);

  NotificationObserverList* observer_list;
  if (HasKey(observers_[type.value], source)) {
    observer_list = observers_[type.value][source.map_key()];
  } else {
    observer_list = new NotificationObserverList;
    observers_[type.value][source.map_key()] = observer_list;
  }

  observer_list->AddObserver(observer);
#ifndef NDEBUG
  ++observer_counts_[type.value];
#endif
}
