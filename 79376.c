  explicit FullFeedFetcher(JobScheduler* scheduler)
      : scheduler_(scheduler),
        weak_ptr_factory_(this) {
  }
