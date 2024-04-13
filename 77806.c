  void SimulateServerError() {
    net::TestURLFetcher* url_fetcher =
        test_url_fetcher_factory_.GetFetcherByID(0);
    ASSERT_NE(nullptr, url_fetcher);

    url_fetcher->set_response_code(500);
    url_fetcher->delegate()->OnURLFetchComplete(url_fetcher);
  }
