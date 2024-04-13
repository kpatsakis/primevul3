ServiceStartInteractive(DWORD dwArgc, LPTSTR *lpszArgv)
{
    HANDLE pipe, io_event = NULL;
    OVERLAPPED overlapped;
    DWORD error = NO_ERROR;
    list_item_t *threads = NULL;
    PHANDLE handles = NULL;
    DWORD handle_count;

    service = RegisterServiceCtrlHandlerEx(interactive_service.name, ServiceCtrlInteractive, &status);
    if (!service)
    {
        return;
    }

    status.dwCurrentState = SERVICE_START_PENDING;
    status.dwServiceSpecificExitCode = NO_ERROR;
    status.dwWin32ExitCode = NO_ERROR;
    status.dwWaitHint = 3000;
    ReportStatusToSCMgr(service, &status);

    /* Read info from registry in key HKLM\SOFTWARE\OpenVPN */
    error = GetOpenvpnSettings(&settings);
    if (error != ERROR_SUCCESS)
    {
        goto out;
    }

    io_event = InitOverlapped(&overlapped);
    exit_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!exit_event || !io_event)
    {
        error = MsgToEventLog(M_SYSERR, TEXT("Could not create event"));
        goto out;
    }

    rdns_semaphore = CreateSemaphoreW(NULL, 1, 1, NULL);
    if (!rdns_semaphore)
    {
        error = MsgToEventLog(M_SYSERR, TEXT("Could not create semaphore for register-dns"));
        goto out;
    }

    error = UpdateWaitHandles(&handles, &handle_count, io_event, exit_event, threads);
    if (error != NO_ERROR)
    {
        goto out;
    }

    pipe = CreateClientPipeInstance();
    if (pipe == INVALID_HANDLE_VALUE)
    {
        goto out;
    }

    status.dwCurrentState = SERVICE_RUNNING;
    status.dwWaitHint = 0;
    ReportStatusToSCMgr(service, &status);

    while (TRUE)
    {
        if (ConnectNamedPipe(pipe, &overlapped) == FALSE
            && GetLastError() != ERROR_PIPE_CONNECTED
            && GetLastError() != ERROR_IO_PENDING)
        {
            MsgToEventLog(M_SYSERR, TEXT("Could not connect pipe"));
            break;
        }

        error = WaitForMultipleObjects(handle_count, handles, FALSE, INFINITE);
        if (error == WAIT_OBJECT_0)
        {
            /* Client connected, spawn a worker thread for it */
            HANDLE next_pipe = CreateClientPipeInstance();
            HANDLE thread = CreateThread(NULL, 0, RunOpenvpn, pipe, CREATE_SUSPENDED, NULL);
            if (thread)
            {
                error = AddListItem(&threads, thread);
                if (!error)
                {
                    error = UpdateWaitHandles(&handles, &handle_count, io_event, exit_event, threads);
                }
                if (error)
                {
                    ReturnError(pipe, error, L"Insufficient resources to service new clients", 1, &exit_event);
                    /* Update wait handles again after removing the last worker thread */
                    RemoveListItem(&threads, CmpHandle, thread);
                    UpdateWaitHandles(&handles, &handle_count, io_event, exit_event, threads);
                    TerminateThread(thread, 1);
                    CloseHandleEx(&thread);
                    CloseHandleEx(&pipe);
                }
                else
                {
                    ResumeThread(thread);
                }
            }
            else
            {
                CloseHandleEx(&pipe);
            }

            ResetOverlapped(&overlapped);
            pipe = next_pipe;
        }
        else
        {
            CancelIo(pipe);
            if (error == WAIT_FAILED)
            {
                MsgToEventLog(M_SYSERR, TEXT("WaitForMultipleObjects failed"));
                SetEvent(exit_event);
                /* Give some time for worker threads to exit and then terminate */
                Sleep(1000);
                break;
            }
            if (!threads)
            {
                /* exit event signaled */
                CloseHandleEx(&pipe);
                ResetEvent(exit_event);
                error = NO_ERROR;
                break;
            }

            /* Worker thread ended */
            HANDLE thread = RemoveListItem(&threads, CmpHandle, handles[error]);
            UpdateWaitHandles(&handles, &handle_count, io_event, exit_event, threads);
            CloseHandleEx(&thread);
        }
    }

out:
    FreeWaitHandles(handles);
    CloseHandleEx(&io_event);
    CloseHandleEx(&exit_event);
    CloseHandleEx(&rdns_semaphore);

    status.dwCurrentState = SERVICE_STOPPED;
    status.dwWin32ExitCode = error;
    ReportStatusToSCMgr(service, &status);
}
