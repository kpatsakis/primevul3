  FastFetchFeedFetcher(JobScheduler* scheduler,
                       DriveServiceInterface* drive_service,
                       const std::string& directory_resource_id,
                       const std::string& root_folder_id)
      : scheduler_(scheduler),
        drive_service_(drive_service),
        directory_resource_id_(directory_resource_id),
        root_folder_id_(root_folder_id),
        weak_ptr_factory_(this) {
  }
