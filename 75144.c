void SetExemplarSetForLang(const std::string& lang,
                           icu::UnicodeSet* lang_set) {
  LangToExemplarSetMap& map = Singleton<LangToExemplarSet>()->map;
  map.insert(std::make_pair(lang, lang_set));
}
