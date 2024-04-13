AsyncPipeOp(async_op_t op, HANDLE pipe, LPVOID buffer, DWORD size, DWORD count, LPHANDLE events)
{
    int i;
    BOOL success;
    HANDLE io_event;
    DWORD res, bytes = 0;
    OVERLAPPED overlapped;
    LPHANDLE handles = NULL;

    io_event = InitOverlapped(&overlapped);
    if (!io_event)
    {
        goto out;
    }

    handles = malloc((count + 1) * sizeof(HANDLE));
    if (!handles)
    {
        goto out;
    }

    if (op == write)
    {
        success = WriteFile(pipe, buffer, size, NULL, &overlapped);
    }
    else
    {
        success = ReadFile(pipe, buffer, size, NULL, &overlapped);
    }
    if (!success && GetLastError() != ERROR_IO_PENDING && GetLastError() != ERROR_MORE_DATA)
    {
        goto out;
    }

    handles[0] = io_event;
    for (i = 0; i < count; i++)
    {
        handles[i + 1] = events[i];
    }

    res = WaitForMultipleObjects(count + 1, handles, FALSE,
                                 op == peek ? INFINITE : IO_TIMEOUT);
    if (res != WAIT_OBJECT_0)
    {
        CancelIo(pipe);
        goto out;
    }

    if (op == peek)
    {
        PeekNamedPipe(pipe, NULL, 0, NULL, &bytes, NULL);
    }
    else
    {
        GetOverlappedResult(pipe, &overlapped, &bytes, TRUE);
    }

out:
    CloseHandleEx(&io_event);
    free(handles);
    return bytes;
}
