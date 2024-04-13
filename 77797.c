GURL AffiliationFetcher::BuildQueryURL() const {
  return net::AppendQueryParameter(
      GURL("https://www.googleapis.com/affiliation/v1/affiliation:lookup"),
      "key", google_apis::GetAPIKey());
}
