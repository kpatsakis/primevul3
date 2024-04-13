std::string GetHostAndPort(const GURL& url) {
  return StringPrintf("%s:%d", url.host().c_str(), url.EffectiveIntPort());
}
