  scoped_refptr<HttpProxySocketParams> GetParams(bool tunnel) {
    return scoped_refptr<HttpProxySocketParams>(new HttpProxySocketParams(
        GetTcpParams(),
        GetSslParams(),
        GURL(tunnel ? "https://www.google.com/" : "http://www.google.com"),
        std::string(),
        HostPortPair("www.google.com", tunnel ? 443 : 80),
        session_->http_auth_cache(),
        session_->http_auth_handler_factory(),
        session_->spdy_session_pool(),
        tunnel));
  }
