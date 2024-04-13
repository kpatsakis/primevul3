ReadPipeAsync(HANDLE pipe, LPVOID buffer, DWORD size, DWORD count, LPHANDLE events)
{
    return AsyncPipeOp(read, pipe, buffer, size, count, events);
}
