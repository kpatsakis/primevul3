void DelegatedFrameHost::BeginFrameSubscription(
    scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber) {
  frame_subscriber_ = subscriber.Pass();
}
