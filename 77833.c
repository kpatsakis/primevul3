void RenderFrameImpl::Initialize() {
#if defined(ENABLE_PLUGINS)
  new PepperBrowserConnection(this);
#endif
  new SharedWorkerRepository(this);

  if (!frame_->parent())
    new ImageLoadingHelper(this);

  GetContentClient()->renderer()->RenderFrameCreated(this);
}
