bool fileExists(const String& path)
{
    WIN32_FIND_DATAW findData;
    return getFindData(path, findData);
}
