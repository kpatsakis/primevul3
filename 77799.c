std::string AffiliationFetcher::PreparePayload() const {
  affiliation_pb::LookupAffiliationRequest lookup_request;
  for (const FacetURI& uri : requested_facet_uris_)
    lookup_request.add_facet(uri.canonical_spec());

  std::string serialized_request;
  bool success = lookup_request.SerializeToString(&serialized_request);
  DCHECK(success);
  return serialized_request;
}
