std::string GetHostAndOptionalPort(const GURL& url) {
  if (url.has_port())
    return StringPrintf("%s:%s", url.host().c_str(), url.port().c_str());
  return url.host();
}
