bool RenderView::IsPageTranslatable(WebKit::WebDocument* document) {
  std::vector<WebKit::WebElement> meta_elements;
  webkit_glue::GetMetaElementsWithName(document, ASCIIToUTF16("google"),
                                       &meta_elements);
  std::vector<WebKit::WebElement>::const_iterator iter;
  for (iter = meta_elements.begin(); iter != meta_elements.end(); ++iter) {
    WebString attribute = iter->getAttribute("value");
    if (attribute.isNull())  // We support both 'value' and 'content'.
      attribute = iter->getAttribute("content");
    if (attribute.isNull())
      continue;
    if (LowerCaseEqualsASCII(attribute, "notranslate"))
      return false;
  }
  return true;
}
