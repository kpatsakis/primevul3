RegisterDNS(LPVOID unused)
{
    DWORD err;
    DWORD i;
    WCHAR sys_path[MAX_PATH];
    DWORD timeout = RDNS_TIMEOUT * 1000; /* in milliseconds */

    /* default path of ipconfig command */
    WCHAR ipcfg[MAX_PATH] = L"C:\\Windows\\system32\\ipconfig.exe";

    struct
    {
        WCHAR *argv0;
        WCHAR *cmdline;
        DWORD timeout;
    } cmds [] = {
        { ipcfg, L"ipconfig /flushdns",    timeout },
        { ipcfg, L"ipconfig /registerdns", timeout },
    };
    int ncmds = sizeof(cmds) / sizeof(cmds[0]);

    HANDLE wait_handles[2] = {rdns_semaphore, exit_event};

    if (GetSystemDirectory(sys_path, MAX_PATH))
    {
        swprintf(ipcfg, MAX_PATH, L"%s\\%s", sys_path, L"ipconfig.exe");
        ipcfg[MAX_PATH-1] = L'\0';
    }

    if (WaitForMultipleObjects(2, wait_handles, FALSE, timeout) == WAIT_OBJECT_0)
    {
        /* Semaphore locked */
        for (i = 0; i < ncmds; ++i)
        {
            ExecCommand(cmds[i].argv0, cmds[i].cmdline, cmds[i].timeout);
        }
        err = 0;
        if (!ReleaseSemaphore(rdns_semaphore, 1, NULL) )
        {
            err = MsgToEventLog(M_SYSERR, TEXT("RegisterDNS: Failed to release regsiter-dns semaphore:"));
        }
    }
    else
    {
        MsgToEventLog(M_ERR, TEXT("RegisterDNS: Failed to lock register-dns semaphore"));
        err = ERROR_SEM_TIMEOUT; /* Windows error code 0x79 */
    }
    return err;
}
