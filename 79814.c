  SafeBrowsingBlockingPage* CreateSafeBrowsingPage(
      SafeBrowsingUIManager* ui_manager,
      WebContents* web_contents,
      const SafeBrowsingBlockingPage::UnsafeResourceList& unsafe_resources) {
    if (unsafe_resources.size() == 1 &&
        (unsafe_resources[0].threat_type == SB_THREAT_TYPE_URL_MALWARE ||
         unsafe_resources[0].threat_type == SB_THREAT_TYPE_URL_PHISHING)) {
      return new SafeBrowsingBlockingPageV2(ui_manager, web_contents,
          unsafe_resources);
    }
    return new SafeBrowsingBlockingPageV1(ui_manager, web_contents,
                                          unsafe_resources);
  }
