void WebContentsImpl::AddDestructionObserver(WebContentsImpl* web_contents) {
  if (!ContainsKey(destruction_observers_, web_contents)) {
    destruction_observers_[web_contents] =
        new DestructionObserver(this, web_contents);
  }
}
