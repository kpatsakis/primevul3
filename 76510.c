  void OnNetworkManagerChanged(NetworkLibrary* netlib) {
    if (netlib->Connected()) {
      chrome_browser_net::Preconnect::PreconnectOnUIThread(
          GURL(GaiaAuthenticator2::kClientLoginUrl),
          chrome_browser_net::UrlInfo::EARLY_LOAD_MOTIVATED);
      netlib->RemoveNetworkManagerObserver(this);
      delete this;
    }
  }
