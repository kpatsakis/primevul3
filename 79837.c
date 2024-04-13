bool CanSaveAsComplete(const std::string& contents_mime_type) {
  return contents_mime_type == "text/html" ||
         contents_mime_type == "application/xhtml+xml";
}
