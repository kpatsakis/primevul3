bool IsComponentCoveredByLang(const icu::UnicodeSet& component_characters,
                              const std::string& lang) {
  static const icu::UnicodeSet kASCIILetters(0x61, 0x7a);  // [a-z]
  icu::UnicodeSet* lang_set;
  {
    AutoLock lock(lang_set_lock);
    if (!GetExemplarSetForLang(lang, &lang_set)) {
      UErrorCode status = U_ZERO_ERROR;
      ULocaleData* uld = ulocdata_open(lang.c_str(), &status);
      if (U_SUCCESS(status) && status != U_USING_DEFAULT_WARNING) {
        lang_set = reinterpret_cast<icu::UnicodeSet *>(
            ulocdata_getExemplarSet(uld, NULL, 0,
                                    ULOCDATA_ES_STANDARD, &status));
        if (IsCompatibleWithASCIILetters(lang))
          lang_set->addAll(kASCIILetters);
      } else {
        lang_set = new icu::UnicodeSet(1, 0);
      }
      lang_set->freeze();
      SetExemplarSetForLang(lang, lang_set);
      ulocdata_close(uld);
    }
  }
  return !lang_set->isEmpty() && lang_set->containsAll(component_characters);
}
