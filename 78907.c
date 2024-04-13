Locale& Element::locale() const
{
    return document()->getCachedLocale(computeInheritedLanguage());
}
