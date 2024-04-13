bool HttpStreamParser::ShouldMergeRequestHeadersAndBody(
    const std::string& request_headers,
    const UploadDataStream* request_body) {
  if (request_body != NULL &&
      request_body->IsInMemory() &&
      request_body->size() > 0) {
    size_t merged_size = request_headers.size() + request_body->size();
    if (merged_size <= kMaxMergedHeaderAndBodySize)
      return true;
  }
  return false;
}
