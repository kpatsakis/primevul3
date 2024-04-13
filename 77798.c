void AffiliationFetcher::OnURLFetchComplete(const net::URLFetcher* source) {
  DCHECK_EQ(source, fetcher_.get());

  scoped_ptr<AffiliationFetcherDelegate::Result> result(
      new AffiliationFetcherDelegate::Result);
  if (fetcher_->GetStatus().status() == net::URLRequestStatus::SUCCESS &&
      fetcher_->GetResponseCode() == net::HTTP_OK) {
    if (ParseResponse(result.get()))
      delegate_->OnFetchSucceeded(result.Pass());
    else
      delegate_->OnMalformedResponse();
  } else {
    delegate_->OnFetchFailed();
  }
}
