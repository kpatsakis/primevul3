  void ServiceURLRequest(const std::string& response) {
    net::TestURLFetcher* url_fetcher =
        test_url_fetcher_factory_.GetFetcherByID(0);
    ASSERT_NE(nullptr, url_fetcher);

    url_fetcher->set_response_code(200);
    url_fetcher->SetResponseString(response);
    url_fetcher->delegate()->OnURLFetchComplete(url_fetcher);
  }
