void PageClickTracker::AddListener(PageClickListener* listener) {
  listeners_.AddObserver(listener);
}
