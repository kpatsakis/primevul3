int writeToFile(PlatformFileHandle handle, const char* data, int length)
{
    if (!isHandleValid(handle))
        return -1;

    DWORD bytesWritten;
    bool success = WriteFile(handle, data, length, &bytesWritten, 0);

    if (!success)
        return -1;
    return static_cast<int>(bytesWritten);
}
