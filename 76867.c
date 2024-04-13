FileSystemOperation::FileSystemOperation(
    FileSystemContext* file_system_context)
    : operation_context_(file_system_context),
      src_util_(NULL),
      dest_util_(NULL),
      peer_handle_(base::kNullProcessHandle),
      pending_operation_(kOperationNone),
      weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {
}
