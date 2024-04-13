void WebContentsImpl::SetEncoding(const std::string& encoding) {
  encoding_ = GetContentClient()->browser()->
      GetCanonicalEncodingNameByAliasName(encoding);
}
