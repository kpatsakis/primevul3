void LoginUtilsImpl::PrewarmAuthentication() {
  if (CrosLibrary::Get()->EnsureLoaded()) {
    NetworkLibrary *network = CrosLibrary::Get()->GetNetworkLibrary();
    if (network->Connected()) {
      chrome_browser_net::Preconnect::PreconnectOnUIThread(
          GURL(GaiaAuthenticator2::kClientLoginUrl),
          chrome_browser_net::UrlInfo::EARLY_LOAD_MOTIVATED);
    } else {
      new WarmingObserver();
    }
  }
}
