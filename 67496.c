ResetOverlapped(LPOVERLAPPED overlapped)
{
    HANDLE io_event = overlapped->hEvent;
    if (!ResetEvent(io_event))
    {
        return FALSE;
    }
    ZeroMemory(overlapped, sizeof(OVERLAPPED));
    overlapped->hEvent = io_event;
    return TRUE;
}
