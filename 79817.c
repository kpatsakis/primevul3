std::string SafeBrowsingBlockingPageV2::GetHTMLContents() {
  DictionaryValue strings;
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  std::string html;

  if (unsafe_resources_.empty()) {
    NOTREACHED();
    return std::string();
  }

  if (unsafe_resources_.size() > 1) {
    NOTREACHED();
  } else {
    SBThreatType threat_type = unsafe_resources_[0].threat_type;
    if (threat_type == SB_THREAT_TYPE_URL_MALWARE) {
      PopulateMalwareStringDictionary(&strings);
    } else {  // Phishing.
      DCHECK(threat_type == SB_THREAT_TYPE_URL_PHISHING ||
             threat_type == SB_THREAT_TYPE_CLIENT_SIDE_PHISHING_URL);
      PopulatePhishingStringDictionary(&strings);
    }
    html = rb.GetRawDataResource(IDR_SAFE_BROWSING_MALWARE_BLOCK_V2).
        as_string();
  }
  interstitial_show_time_ = base::TimeTicks::Now();
  return jstemplate_builder::GetTemplatesHtml(html, &strings, "template-root");
}
