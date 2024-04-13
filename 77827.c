void GetRedirectChain(WebDataSource* ds, std::vector<GURL>* result) {
  const WebURL& blank_url = GURL(kAboutBlankURL);
  WebVector<WebURL> urls;
  ds->redirectChain(urls);
  result->reserve(urls.size());
  for (size_t i = 0; i < urls.size(); ++i) {
    if (urls[i] != GURL(kSwappedOutURL))
      result->push_back(urls[i]);
    else
      result->push_back(blank_url);
  }
}
