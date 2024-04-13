String openTemporaryFile(const String&, PlatformFileHandle& handle)
{
    handle = INVALID_HANDLE_VALUE;

    wchar_t tempPath[MAX_PATH];
    int tempPathLength = ::GetTempPathW(WTF_ARRAY_LENGTH(tempPath), tempPath);
    if (tempPathLength <= 0 || tempPathLength > WTF_ARRAY_LENGTH(tempPath))
        return String();

    String proposedPath;
    do {
        wchar_t tempFile[] = L"XXXXXXXX.tmp"; // Use 8.3 style name (more characters aren't helpful due to 8.3 short file names)
        const int randomPartLength = 8;
        cryptographicallyRandomValues(tempFile, randomPartLength * sizeof(wchar_t));

        const char validChars[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < randomPartLength; ++i)
            tempFile[i] = validChars[tempFile[i] % (sizeof(validChars) - 1)];

        ASSERT(wcslen(tempFile) == WTF_ARRAY_LENGTH(tempFile) - 1);

        proposedPath = pathByAppendingComponent(tempPath, tempFile);
        if (proposedPath.isEmpty())
            break;

        handle = ::CreateFileW(proposedPath.charactersWithNullTermination(), GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
    } while (!isHandleValid(handle) && GetLastError() == ERROR_ALREADY_EXISTS);

    if (!isHandleValid(handle))
        return String();

    return proposedPath;
}
