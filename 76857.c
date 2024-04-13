 void FileSystemOperation::DidGetUsageAndQuotaAndRunTask(
    const TaskParamsForDidGetQuota& params,
    quota::QuotaStatusCode status,
    int64 usage, int64 quota) {
  if (status != quota::kQuotaStatusOk) {
    LOG(WARNING) << "Got unexpected quota error : " << status;
    params.error_callback.Run();
    return;
  }

  operation_context_.set_allowed_bytes_growth(quota - usage);
  scoped_quota_notifier_.reset(new ScopedQuotaNotifier(
      file_system_context(), params.origin, params.type));

  params.task.Run();
}
