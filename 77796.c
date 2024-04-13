AffiliationFetcher::AffiliationFetcher(
    net::URLRequestContextGetter* request_context_getter,
    const std::vector<FacetURI>& facet_uris,
    AffiliationFetcherDelegate* delegate)
    : request_context_getter_(request_context_getter),
      requested_facet_uris_(facet_uris),
      delegate_(delegate) {
  for (const FacetURI& uri : requested_facet_uris_) {
    DCHECK(uri.is_valid());
  }
}
