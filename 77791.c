void CurrentHistoryCleaner::WebContentsDestroyed(
    content::WebContents* contents) {
  delete this;  // Failure.
}
