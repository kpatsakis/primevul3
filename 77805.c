  void SimulateNetworkError() {
    net::TestURLFetcher* url_fetcher =
        test_url_fetcher_factory_.GetFetcherByID(0);
    ASSERT_NE(nullptr, url_fetcher);
    url_fetcher->set_status(net::URLRequestStatus(net::URLRequestStatus::FAILED,
                                                  net::ERR_NETWORK_CHANGED));
    url_fetcher->delegate()->OnURLFetchComplete(url_fetcher);
  }
