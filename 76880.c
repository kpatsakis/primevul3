FileSystemOperation::ScopedQuotaNotifier::~ScopedQuotaNotifier() {
  if (quota_util_) {
    DCHECK(quota_util_->proxy());
    quota_util_->proxy()->EndUpdateOrigin(origin_url_, type_);
  }
}
