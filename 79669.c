static bool isValidMIMEType(const String& type)
{
    size_t slashPosition = type.find('/');
    if (slashPosition == kNotFound || !slashPosition || slashPosition == type.length() - 1)
        return false;
    for (size_t i = 0; i < type.length(); ++i) {
        if (!isRFC2616TokenCharacter(type[i]) && i != slashPosition)
            return false;
    }
    return true;
}
