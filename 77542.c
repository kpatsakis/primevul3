  explicit SyncInternal(const std::string& name)
      : name_(name),
        weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
        registrar_(NULL),
        change_delegate_(NULL),
        initialized_(false),
        setup_for_test_mode_(false),
        observing_ip_address_changes_(false),
        unrecoverable_error_handler_(NULL),
        created_on_loop_(MessageLoop::current()) {
    for (int i = syncable::FIRST_REAL_MODEL_TYPE;
         i < syncable::MODEL_TYPE_COUNT; ++i) {
      notification_info_map_.insert(
          std::make_pair(syncable::ModelTypeFromInt(i), NotificationInfo()));
    }

    BindJsMessageHandler(
        "getNotificationState",
        &SyncManager::SyncInternal::GetNotificationState);
    BindJsMessageHandler(
        "getNotificationInfo",
        &SyncManager::SyncInternal::GetNotificationInfo);
    BindJsMessageHandler(
        "getRootNodeDetails",
        &SyncManager::SyncInternal::GetRootNodeDetails);
    BindJsMessageHandler(
        "getNodeSummariesById",
        &SyncManager::SyncInternal::GetNodeSummariesById);
    BindJsMessageHandler(
        "getNodeDetailsById",
        &SyncManager::SyncInternal::GetNodeDetailsById);
    BindJsMessageHandler(
        "getChildNodeIds",
        &SyncManager::SyncInternal::GetChildNodeIds);
    BindJsMessageHandler(
        "findNodesContainingString",
        &SyncManager::SyncInternal::FindNodesContainingString);
  }
