  scoped_refptr<SSLSocketParams> GetSslParams() {
    if (GetParam() == HTTP)
      return scoped_refptr<SSLSocketParams>();
    return ignored_ssl_socket_params_;
  }
