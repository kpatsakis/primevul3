void HttpStreamParser::GetSSLInfo(SSLInfo* ssl_info) {
  if (request_->url.SchemeIs("https") && connection_->socket()) {
    SSLClientSocket* ssl_socket =
        static_cast<SSLClientSocket*>(connection_->socket());
    ssl_socket->GetSSLInfo(ssl_info);
  }
}
