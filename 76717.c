bool safeCreateFile(const String& path, CFDataRef data)
{
    WCHAR tempDirPath[MAX_PATH];
    if (!GetTempPathW(WTF_ARRAY_LENGTH(tempDirPath), tempDirPath))
        return false;

    WCHAR tempPath[MAX_PATH];
    if (!GetTempFileNameW(tempDirPath, L"WEBKIT", 0, tempPath))
        return false;

    HANDLE tempFileHandle = CreateFileW(tempPath, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (tempFileHandle == INVALID_HANDLE_VALUE)
        return false;

    DWORD written;
    if (!WriteFile(tempFileHandle, CFDataGetBytePtr(data), static_cast<DWORD>(CFDataGetLength(data)), &written, 0))
        return false;

    CloseHandle(tempFileHandle);

    String destination = path;
    if (!MoveFileExW(tempPath, destination.charactersWithNullTermination(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
        return false;

    return true;
}
