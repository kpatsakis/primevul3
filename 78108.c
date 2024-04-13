bool WebContentsImpl::IsSavable() {
  return contents_mime_type_ == "text/html" ||
         contents_mime_type_ == "text/xml" ||
         contents_mime_type_ == "application/xhtml+xml" ||
         contents_mime_type_ == "text/plain" ||
         contents_mime_type_ == "text/css" ||
         net::IsSupportedJavascriptMimeType(contents_mime_type_.c_str());
}
