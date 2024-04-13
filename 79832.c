void SafeBrowsingBlockingPage::RecordUserReactionTime(
    const std::string& command) {
  if (interstitial_show_time_.is_null())
    return;  // We already reported the user reaction time.
  base::TimeDelta dt = base::TimeTicks::Now() - interstitial_show_time_;
  DVLOG(1) << "User reaction time for command:" << command
           << " on interstitial_type_:" << interstitial_type_
           << " warning took " << dt.InMilliseconds() << "ms";
  bool recorded = true;
  if (interstitial_type_ == TYPE_MALWARE ||
      interstitial_type_ == TYPE_MALWARE_AND_PHISHING) {
    if (command == kProceedCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialTimeProceed", dt);
    } else if (command == kTakeMeBackCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialTimeTakeMeBack", dt);
    } else if (command == kShowDiagnosticCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialTimeDiagnostic", dt);
    } else if (command == kShowPrivacyCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialTimePrivacyPolicy",
                                 dt);
    } else if (command == kLearnMoreCommand || command == kLearnMoreCommandV2) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialLearnMore",
                                 dt);
    } else if (command == kNavigatedAwayMetaCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialTimeClosed", dt);
    } else if (command == kExpandedSeeMore) {
      if (has_expanded_see_more_section_)
        return;

      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.MalwareInterstitialTimeExpandedSeeMore",
                                 dt);
      has_expanded_see_more_section_ = true;
      recorded = false;
    } else {
      recorded = false;
    }
  } else {
    if (command == kProceedCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.PhishingInterstitialTimeProceed", dt);
    } else if (command == kTakeMeBackCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.PhishingInterstitialTimeTakeMeBack", dt);
    } else if (command == kShowDiagnosticCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.PhishingInterstitialTimeReportError", dt);
    } else if (command == kLearnMoreCommand || command == kLearnMoreCommandV2) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.PhishingInterstitialTimeLearnMore", dt);
    } else if (command == kNavigatedAwayMetaCommand) {
      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.PhishingInterstitialTimeClosed", dt);
    } else if (command == kExpandedSeeMore) {
      if (has_expanded_see_more_section_)
        return;

      UMA_HISTOGRAM_MEDIUM_TIMES("SB2.PhishingInterstitialTimeExpandedSeeMore",
                                 dt);
      has_expanded_see_more_section_ = true;
      recorded = false;
    } else {
      recorded = false;
    }
  }
  if (recorded)  // Making sure we don't double-count reaction times.
    interstitial_show_time_ = base::TimeTicks();  //  Resets the show time.
}
