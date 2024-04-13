PlatformFileHandle openFile(const String& path, FileOpenMode mode)
{
    DWORD desiredAccess = 0;
    DWORD creationDisposition = 0;
    switch (mode) {
    case OpenForRead:
        desiredAccess = GENERIC_READ;
        creationDisposition = OPEN_EXISTING;
        break;
    case OpenForWrite:
        desiredAccess = GENERIC_WRITE;
        creationDisposition = CREATE_ALWAYS;
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    String destination = path;
    return CreateFile(destination.charactersWithNullTermination(), desiredAccess, 0, 0, creationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
}
