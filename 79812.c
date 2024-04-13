void VisitCountsToVisitedBefore(
    const VisitedBeforeCallback& callback,
    HistoryService::Handle unused_handle,
    bool found_visits,
    int count,
    base::Time first_visit) {
  callback.Run(found_visits && count &&
      (first_visit.LocalMidnight() < base::Time::Now().LocalMidnight()));
}
