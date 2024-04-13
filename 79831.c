void SafeBrowsingBlockingPage::RecordUserAction(BlockingPageEvent event) {
  enum {
    MALWARE_SHOW = 0,
    MALWARE_DONT_PROCEED,
    MALWARE_FORCED_DONT_PROCEED,
    MALWARE_PROCEED,
    MULTIPLE_SHOW,
    MULTIPLE_DONT_PROCEED,
    MULTIPLE_FORCED_DONT_PROCEED,
    MULTIPLE_PROCEED,
    PHISHING_SHOW,
    PHISHING_DONT_PROCEED,
    PHISHING_FORCED_DONT_PROCEED,
    PHISHING_PROCEED,
    MAX_ACTION
  } histogram_action = MAX_ACTION;

  switch (event) {
    case SHOW:
      switch (interstitial_type_) {
        case TYPE_MALWARE_AND_PHISHING:
          histogram_action = MULTIPLE_SHOW;
          break;
        case TYPE_MALWARE:
          histogram_action = MALWARE_SHOW;
          break;
        case TYPE_PHISHING:
          histogram_action = PHISHING_SHOW;
          break;
      }
      break;
    case PROCEED:
      switch (interstitial_type_) {
        case TYPE_MALWARE_AND_PHISHING:
          histogram_action = MULTIPLE_PROCEED;
          break;
        case TYPE_MALWARE:
          histogram_action = MALWARE_PROCEED;
          break;
        case TYPE_PHISHING:
          histogram_action = PHISHING_PROCEED;
          break;
      }
      break;
    case DONT_PROCEED:
      if (IsPrefEnabled(prefs::kSafeBrowsingProceedAnywayDisabled)) {
        switch (interstitial_type_) {
          case TYPE_MALWARE_AND_PHISHING:
            histogram_action = MULTIPLE_FORCED_DONT_PROCEED;
            break;
          case TYPE_MALWARE:
            histogram_action = MALWARE_FORCED_DONT_PROCEED;
            break;
          case TYPE_PHISHING:
            histogram_action = PHISHING_FORCED_DONT_PROCEED;
            break;
        }
      } else {
        switch (interstitial_type_) {
          case TYPE_MALWARE_AND_PHISHING:
            histogram_action = MULTIPLE_DONT_PROCEED;
            break;
          case TYPE_MALWARE:
            histogram_action = MALWARE_DONT_PROCEED;
            break;
          case TYPE_PHISHING:
            histogram_action = PHISHING_DONT_PROCEED;
            break;
        }
      }
      break;
    default:
      NOTREACHED() << "Unexpected event: " << event;
  }
  if (histogram_action == MAX_ACTION) {
    NOTREACHED();
  } else {
    UMA_HISTOGRAM_ENUMERATION("SB2.InterstitialAction", histogram_action,
                              MAX_ACTION);
  }

  std::string action = "SBInterstitial";
  switch (interstitial_type_) {
    case TYPE_MALWARE_AND_PHISHING:
      action.append("Multiple");
      break;
    case TYPE_MALWARE:
      action.append("Malware");
      break;
    case TYPE_PHISHING:
      action.append("Phishing");
      break;
  }

  switch (event) {
    case SHOW:
      action.append("Show");
      break;
    case PROCEED:
      action.append("Proceed");
      break;
    case DONT_PROCEED:
      if (IsPrefEnabled(prefs::kSafeBrowsingProceedAnywayDisabled))
        action.append("ForcedDontProceed");
      else
        action.append("DontProceed");
      break;
    default:
      NOTREACHED() << "Unexpected event: " << event;
  }

  content::RecordComputedAction(action);
}
