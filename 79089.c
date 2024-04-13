void WebURLLoaderImpl::Context::OnCompletedRequest(
    int error_code,
    bool was_ignored_by_handler,
    const std::string& security_info,
    const base::TimeTicks& completion_time) {
  if (ftp_listing_delegate_) {
    ftp_listing_delegate_->OnCompletedRequest();
    ftp_listing_delegate_.reset(NULL);
  } else if (multipart_delegate_) {
    multipart_delegate_->OnCompletedRequest();
    multipart_delegate_.reset(NULL);
  }

  DCHECK(!completed_bridge_.get());
  completed_bridge_.swap(bridge_);

  if (client_) {
    if (error_code != net::OK) {
      client_->didFail(loader_, CreateError(request_.url(), error_code));
    } else {
      client_->didFinishLoading(
          loader_, (completion_time - TimeTicks()).InSecondsF());
    }
  }

  Release();
}
