  void VerifyRequestPayload(const std::vector<FacetURI>& expected_facet_uris) {
    net::TestURLFetcher* url_fetcher =
        test_url_fetcher_factory_.GetFetcherByID(0);
    ASSERT_NE(nullptr, url_fetcher);

    affiliation_pb::LookupAffiliationRequest request;
    ASSERT_TRUE(request.ParseFromString(url_fetcher->upload_data()));

    std::vector<FacetURI> actual_uris;
    for (int i = 0; i < request.facet_size(); ++i)
      actual_uris.push_back(FacetURI::FromCanonicalSpec(request.facet(i)));

    EXPECT_EQ("application/x-protobuf", url_fetcher->upload_content_type());
    EXPECT_THAT(actual_uris,
                testing::UnorderedElementsAreArray(expected_facet_uris));
  }
