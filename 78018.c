void WebContentsImpl::DidGetResourceResponseStart(
  const ResourceRequestDetails& details) {
  controller_.ssl_manager()->DidStartResourceResponse(details);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidGetResourceResponseStart(details));

  NotificationService::current()->Notify(
      NOTIFICATION_RESOURCE_RESPONSE_STARTED,
      Source<WebContents>(this),
      Details<const ResourceRequestDetails>(&details));
}
