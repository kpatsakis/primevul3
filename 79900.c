bool ScriptLoader::isScriptTypeSupported(LegacyTypeSupport supportLegacyTypes) const
{

    String type = client()->typeAttributeValue();
    String language = client()->languageAttributeValue();
    if (type.isEmpty() && language.isEmpty())
        return true; // Assume text/javascript.
    if (type.isEmpty()) {
        type = "text/" + language.lower();
        if (MIMETypeRegistry::isSupportedJavaScriptMIMEType(type) || isLegacySupportedJavaScriptLanguage(language))
            return true;
    } else if (MIMETypeRegistry::isSupportedJavaScriptMIMEType(type.stripWhiteSpace()) || (supportLegacyTypes == AllowLegacyTypeInTypeAttribute && isLegacySupportedJavaScriptLanguage(type))) {
        return true;
    }

    return false;
}
