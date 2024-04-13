NotificationService::NotificationService() {
  DCHECK(current() == NULL);
#ifndef NDEBUG
  memset(observer_counts_, 0, sizeof(observer_counts_));
#endif

  lazy_tls_ptr.Pointer()->Set(this);
}
