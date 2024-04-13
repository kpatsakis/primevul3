SafeBrowsingBlockingPage::SafeBrowsingBlockingPage(
    SafeBrowsingUIManager* ui_manager,
    WebContents* web_contents,
    const UnsafeResourceList& unsafe_resources)
    : malware_details_proceed_delay_ms_(
          kMalwareDetailsProceedDelayMilliSeconds),
      ui_manager_(ui_manager),
      report_loop_(NULL),
      is_main_frame_load_blocked_(IsMainPageLoadBlocked(unsafe_resources)),
      unsafe_resources_(unsafe_resources),
      proceeded_(false),
      web_contents_(web_contents),
      url_(unsafe_resources[0].url),
      has_expanded_see_more_section_(false) {
  bool malware = false;
  bool phishing = false;
  for (UnsafeResourceList::const_iterator iter = unsafe_resources_.begin();
       iter != unsafe_resources_.end(); ++iter) {
    const UnsafeResource& resource = *iter;
    SBThreatType threat_type = resource.threat_type;
    if (threat_type == SB_THREAT_TYPE_URL_MALWARE) {
      malware = true;
    } else {
      DCHECK(threat_type == SB_THREAT_TYPE_URL_PHISHING ||
             threat_type == SB_THREAT_TYPE_CLIENT_SIDE_PHISHING_URL);
      phishing = true;
    }
  }
  DCHECK(phishing || malware);
  if (malware && phishing)
    interstitial_type_ = TYPE_MALWARE_AND_PHISHING;
  else if (malware)
    interstitial_type_ = TYPE_MALWARE;
  else
    interstitial_type_ = TYPE_PHISHING;
  RecordUserAction(SHOW);
  if (!is_main_frame_load_blocked_) {
    navigation_entry_index_to_remove_ =
        web_contents->GetController().GetLastCommittedEntryIndex();
  } else {
    navigation_entry_index_to_remove_ = -1;
  }

  if (unsafe_resources.size() == 1 &&
      unsafe_resources[0].threat_type == SB_THREAT_TYPE_URL_MALWARE &&
      malware_details_ == NULL &&
      CanShowMalwareDetailsOption()) {
    malware_details_ = MalwareDetails::NewMalwareDetails(
        ui_manager_, web_contents, unsafe_resources[0]);
  }

  interstitial_page_ = InterstitialPage::Create(
      web_contents, IsMainPageLoadBlocked(unsafe_resources), url_, this);
}
