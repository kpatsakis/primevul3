ChangeListLoader::ChangeListLoader(
    base::SequencedTaskRunner* blocking_task_runner,
    ResourceMetadata* resource_metadata,
    JobScheduler* scheduler,
    DriveServiceInterface* drive_service)
    : blocking_task_runner_(blocking_task_runner),
      resource_metadata_(resource_metadata),
      scheduler_(scheduler),
      drive_service_(drive_service),
      loaded_(false),
      weak_ptr_factory_(this) {
}
