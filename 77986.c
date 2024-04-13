void WebContentsImpl::BeforeUnloadFiredFromRenderManager(
    bool proceed, const base::TimeTicks& proceed_time,
    bool* proceed_to_fire_unload) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    BeforeUnloadFired(proceed_time));
  if (delegate_)
    delegate_->BeforeUnloadFired(this, proceed, proceed_to_fire_unload);
}
