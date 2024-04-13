void WebContentsImpl::RequestMediaAccessPermission(
    const MediaStreamRequest& request,
    const MediaResponseCallback& callback) {
  if (delegate_)
    delegate_->RequestMediaAccessPermission(this, request, callback);
  else
    callback.Run(MediaStreamDevices(), scoped_ptr<MediaStreamUI>());
}
