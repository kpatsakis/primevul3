void SafeBrowsingBlockingPageV1::PopulateStringDictionary(
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
}
