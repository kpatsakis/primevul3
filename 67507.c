WritePipeAsync(HANDLE pipe, LPVOID data, DWORD size, DWORD count, LPHANDLE events)
{
    return AsyncPipeOp(write, pipe, data, size, count, events);
}
