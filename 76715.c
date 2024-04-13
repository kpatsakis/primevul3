String pathGetFileName(const String& path)
{
    return String(::PathFindFileName(String(path).charactersWithNullTermination()));
}
