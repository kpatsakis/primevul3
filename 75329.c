bool PrintingNodeOrPdfFrame(const WebFrame* frame, const WebNode& node) {
  if (!node.isNull())
    return true;
  std::string mime(frame->dataSource()->response().mimeType().utf8());
  return mime == "application/pdf";
}
