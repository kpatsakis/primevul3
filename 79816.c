std::string SafeBrowsingBlockingPageV1::GetHTMLContents() {
  DictionaryValue strings;
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  std::string html;

  if (unsafe_resources_.empty()) {
    NOTREACHED();
    return std::string();
  }

  DCHECK_GT(unsafe_resources_.size(), 1U);
  PopulateMultipleThreatStringDictionary(&strings);
  html = rb.GetRawDataResource(
      IDR_SAFE_BROWSING_MULTIPLE_THREAT_BLOCK).as_string();
  interstitial_show_time_ = base::TimeTicks::Now();
  return jstemplate_builder::GetTemplatesHtml(html, &strings, "template_root");
}
