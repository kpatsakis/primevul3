void SafeBrowsingBlockingPageV2::PopulateStringDictionary(
    DictionaryValue* strings,
    const string16& title,
    const string16& headline,
    const string16& description1,
    const string16& description2,
    const string16& description3) {
  strings->SetString("title", title);
  strings->SetString("headLine", headline);
  strings->SetString("description1", description1);
  strings->SetString("description2", description2);
  strings->SetString("description3", description3);
  strings->SetBoolean("proceedDisabled",
                      IsPrefEnabled(prefs::kSafeBrowsingProceedAnywayDisabled));
  strings->SetBoolean("isMainFrame", is_main_frame_load_blocked_);
  strings->SetBoolean("isPhishing", interstitial_type_ == TYPE_PHISHING);

  strings->SetString("back_button",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_BACK_BUTTON));
  strings->SetString("seeMore", l10n_util::GetStringUTF16(
      IDS_SAFE_BROWSING_MALWARE_V2_SEE_MORE));
  strings->SetString("proceed",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_V2_PROCEED_LINK));

  URLDataSource::SetFontAndTextDirection(strings);
}
