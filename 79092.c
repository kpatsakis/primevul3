void WebURLLoaderImpl::Context::OnReceivedData(const char* data,
                                               int data_length,
                                               int encoded_data_length) {
  if (!client_)
    return;

  if (ftp_listing_delegate_) {
    ftp_listing_delegate_->OnReceivedData(data, data_length);
  } else if (multipart_delegate_) {
    multipart_delegate_->OnReceivedData(data, data_length, encoded_data_length);
  } else {
    client_->didReceiveData(loader_, data, data_length, encoded_data_length);
  }
}
