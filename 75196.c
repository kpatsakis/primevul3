  virtual void SetUp() {
    URLRequest::RegisterProtocolFactory(chrome::kChromeUIScheme,
                                        &URLRequestTestJob::Factory);
  }
