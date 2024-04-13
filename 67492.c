PeekNamedPipeAsync(HANDLE pipe, DWORD count, LPHANDLE events)
{
    return AsyncPipeOp(peek, pipe, NULL, 0, count, events);
}
