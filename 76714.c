String pathByAppendingComponent(const String& path, const String& component)
{
    Vector<UChar> buffer(MAX_PATH);

    if (path.length() + 1 > buffer.size())
        return String();

    memcpy(buffer.data(), path.characters(), path.length() * sizeof(UChar));
    buffer[path.length()] = '\0';

    String componentCopy = component;
    if (!PathAppendW(buffer.data(), componentCopy.charactersWithNullTermination()))
        return String();

    buffer.resize(wcslen(buffer.data()));

    return String::adopt(buffer);
}
