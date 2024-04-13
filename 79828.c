void SafeBrowsingBlockingPageV2::PopulatePhishingStringDictionary(
    DictionaryValue* strings) {
  PopulateStringDictionary(
      strings,
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_V2_TITLE),
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_V2_HEADLINE),
      l10n_util::GetStringFUTF16(IDS_SAFE_BROWSING_PHISHING_V2_DESCRIPTION1,
                                 l10n_util::GetStringUTF16(IDS_PRODUCT_NAME),
                                 UTF8ToUTF16(url_.host())),
      string16(),
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_V2_DESCRIPTION2));

  strings->SetString("details", "");
  strings->SetString("confirm_text", "");
  strings->SetString(kBoxChecked, "");
  strings->SetString("report_error",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_V2_REPORT_ERROR));
  strings->SetBoolean(kDisplayCheckBox, false);
  strings->SetString("learnMore",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_V2_LEARN_MORE));
}
