ChangeListLoader::~ChangeListLoader() {
  STLDeleteElements(&fast_fetch_feed_fetcher_set_);
}
