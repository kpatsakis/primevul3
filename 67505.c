UpdateWaitHandles(LPHANDLE *handles_ptr, LPDWORD count,
                  HANDLE io_event, HANDLE exit_event, list_item_t *threads)
{
    static DWORD size = 10;
    static LPHANDLE handles = NULL;
    DWORD pos = 0;

    if (handles == NULL)
    {
        handles = malloc(size * sizeof(HANDLE));
        *handles_ptr = handles;
        if (handles == NULL)
        {
            return ERROR_OUTOFMEMORY;
        }
    }

    handles[pos++] = io_event;

    if (!threads)
    {
        handles[pos++] = exit_event;
    }

    while (threads)
    {
        if (pos == size)
        {
            LPHANDLE tmp;
            size += 10;
            tmp = realloc(handles, size * sizeof(HANDLE));
            if (tmp == NULL)
            {
                size -= 10;
                *count = pos;
                return ERROR_OUTOFMEMORY;
            }
            handles = tmp;
            *handles_ptr = handles;
        }
        handles[pos++] = threads->data;
        threads = threads->next;
    }

    *count = pos;
    return NO_ERROR;
}
