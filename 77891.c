blink::WebServiceWorkerProvider* RenderFrameImpl::createServiceWorkerProvider(
    blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  DCHECK(frame->dataSource());
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromDocumentState(
          DocumentState::FromDataSource(frame->dataSource()));
  int provider_id = provider ?
      provider->provider_id() :
      kInvalidServiceWorkerProviderId;
  return new WebServiceWorkerProviderImpl(
      ChildThread::current()->thread_safe_sender(), provider_id);
}
