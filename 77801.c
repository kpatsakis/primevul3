void AffiliationFetcher::StartRequest() {
  DCHECK(!fetcher_);

  fetcher_.reset(
      net::URLFetcher::Create(BuildQueryURL(), net::URLFetcher::POST, this));
  fetcher_->SetRequestContext(request_context_getter_.get());
  fetcher_->SetUploadData("application/x-protobuf", PreparePayload());
  fetcher_->SetLoadFlags(net::LOAD_DO_NOT_SAVE_COOKIES |
                         net::LOAD_DO_NOT_SEND_COOKIES |
                         net::LOAD_DO_NOT_SEND_AUTH_DATA |
                         net::LOAD_BYPASS_CACHE | net::LOAD_DISABLE_CACHE);
  fetcher_->SetAutomaticallyRetryOn5xx(false);
  fetcher_->SetAutomaticallyRetryOnNetworkChanges(0);
  fetcher_->Start();
}
