static GURL GetOriginalRequestURL(WebDataSource* ds) {
  if (ds->hasUnreachableURL())
    return ds->unreachableURL();

  std::vector<GURL> redirects;
  GetRedirectChain(ds, &redirects);
  if (!redirects.empty())
    return redirects.at(0);

  return ds->originalRequest().url();
}
