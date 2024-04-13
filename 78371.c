void RenderThreadImpl::RegisterSchemes() {
  WebString swappedout_scheme(base::ASCIIToUTF16(kSwappedOutScheme));
  WebSecurityPolicy::registerURLSchemeAsDisplayIsolated(swappedout_scheme);
  WebSecurityPolicy::registerURLSchemeAsEmptyDocument(swappedout_scheme);
}
