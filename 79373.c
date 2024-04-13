  DeltaFeedFetcher(JobScheduler* scheduler, int64 start_change_id)
      : scheduler_(scheduler),
        start_change_id_(start_change_id),
        weak_ptr_factory_(this) {
  }
