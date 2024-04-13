void SafeBrowsingBlockingPageV2::PopulateMalwareStringDictionary(
    DictionaryValue* strings) {
  std::string diagnostic_link = base::StringPrintf(kSbDiagnosticHtml,
      l10n_util::GetStringUTF8(
        IDS_SAFE_BROWSING_MALWARE_DIAGNOSTIC_PAGE).c_str());

  string16 headline, description1, description2, description3;


  description3 = l10n_util::GetStringUTF16(
      IDS_SAFE_BROWSING_MALWARE_V2_DESCRIPTION3);
  if (is_main_frame_load_blocked_) {
    headline = l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_V2_HEADLINE);
    description1 = l10n_util::GetStringFUTF16(
        IDS_SAFE_BROWSING_MALWARE_V2_DESCRIPTION1,
        l10n_util::GetStringUTF16(IDS_PRODUCT_NAME),
        UTF8ToUTF16(url_.host()));
    description2 = l10n_util::GetStringUTF16(
        IDS_SAFE_BROWSING_MALWARE_V2_DESCRIPTION2);
    strings->SetString("details", l10n_util::GetStringUTF16(
          IDS_SAFE_BROWSING_MALWARE_V2_DETAILS));
  } else {
    headline = l10n_util::GetStringUTF16(
        IDS_SAFE_BROWSING_MALWARE_V2_HEADLINE_SUBRESOURCE);
    description1 = l10n_util::GetStringFUTF16(
        IDS_SAFE_BROWSING_MALWARE_V2_DESCRIPTION1_SUBRESOURCE,
        l10n_util::GetStringUTF16(IDS_PRODUCT_NAME),
        UTF8ToUTF16(web_contents_->GetURL().host()));
    description2 = l10n_util::GetStringFUTF16(
        IDS_SAFE_BROWSING_MALWARE_V2_DESCRIPTION2_SUBRESOURCE,
        UTF8ToUTF16(url_.host()));
    strings->SetString("details", l10n_util::GetStringFUTF16(
          IDS_SAFE_BROWSING_MALWARE_V2_DETAILS_SUBRESOURCE,
          UTF8ToUTF16(url_.host())));
  }

  PopulateStringDictionary(
      strings,
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_V2_TITLE),
      headline,
      description1,
      description2,
      description3);

  if (!CanShowMalwareDetailsOption()) {
    strings->SetBoolean(kDisplayCheckBox, false);
    strings->SetString("confirm_text", "");
    strings->SetString(kBoxChecked, "");
  } else {
    strings->SetBoolean(kDisplayCheckBox, true);

    std::string privacy_link = base::StringPrintf(
        kPrivacyLinkHtml,
        l10n_util::GetStringUTF8(
            IDS_SAFE_BROWSING_PRIVACY_POLICY_PAGE_V2).c_str());

    strings->SetString("confirm_text",
                       l10n_util::GetStringFUTF16(
                           IDS_SAFE_BROWSING_MALWARE_V2_REPORTING_AGREE,
                           UTF8ToUTF16(privacy_link)));
    if (IsPrefEnabled(prefs::kSafeBrowsingReportingEnabled))
      strings->SetString(kBoxChecked, "yes");
    else
      strings->SetString(kBoxChecked, "");
  }

  strings->SetString("report_error", string16());
  strings->SetString("learnMore",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_V2_LEARN_MORE));
}
