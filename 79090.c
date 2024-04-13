void WebURLLoaderImpl::Context::OnDownloadedData(int len) {
  if (client_)
    client_->didDownloadData(loader_, len);
}
