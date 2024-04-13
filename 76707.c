static bool getFindData(String path, WIN32_FIND_DATAW& findData)
{
    HANDLE handle = FindFirstFileW(path.charactersWithNullTermination(), &findData);
    if (handle == INVALID_HANDLE_VALUE)
        return false;
    FindClose(handle);
    return true;
}
