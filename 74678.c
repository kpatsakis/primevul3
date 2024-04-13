void PageClickTracker::RemoveListener(PageClickListener* listener) {
  listeners_.RemoveObserver(listener);
}
