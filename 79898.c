static bool isLegacySupportedJavaScriptLanguage(const String& language)
{

    typedef HashSet<String, CaseFoldingHash> LanguageSet;
    DEFINE_STATIC_LOCAL(LanguageSet, languages, ());
    if (languages.isEmpty()) {
        languages.add("javascript");
        languages.add("javascript1.0");
        languages.add("javascript1.1");
        languages.add("javascript1.2");
        languages.add("javascript1.3");
        languages.add("javascript1.4");
        languages.add("javascript1.5");
        languages.add("javascript1.6");
        languages.add("javascript1.7");
        languages.add("livescript");
        languages.add("ecmascript");
        languages.add("jscript");
    }

    return languages.contains(language);
}
