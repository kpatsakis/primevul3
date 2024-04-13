InitOverlapped(LPOVERLAPPED overlapped)
{
    ZeroMemory(overlapped, sizeof(OVERLAPPED));
    overlapped->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    return overlapped->hEvent;
}
