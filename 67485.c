ExecCommand(const WCHAR *argv0, const WCHAR *cmdline, DWORD timeout)
{
    DWORD exit_code;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    DWORD proc_flags = CREATE_NO_WINDOW|CREATE_UNICODE_ENVIRONMENT;
    WCHAR *cmdline_dup = NULL;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.cb = sizeof(si);

    /* CreateProcess needs a modifiable cmdline: make a copy */
    cmdline_dup = wcsdup(cmdline);
    if (cmdline_dup && CreateProcessW(argv0, cmdline_dup, NULL, NULL, FALSE,
                                      proc_flags, NULL, NULL, &si, &pi) )
    {
        WaitForSingleObject(pi.hProcess, timeout ? timeout : INFINITE);
        if (!GetExitCodeProcess(pi.hProcess, &exit_code))
        {
            MsgToEventLog(M_SYSERR, TEXT("ExecCommand: Error getting exit_code:"));
            exit_code = GetLastError();
        }
        else if (exit_code == STILL_ACTIVE)
        {
            exit_code = WAIT_TIMEOUT; /* Windows error code 0x102 */

            /* kill without impunity */
            TerminateProcess(pi.hProcess, exit_code);
            MsgToEventLog(M_ERR, TEXT("ExecCommand: \"%s %s\" killed after timeout"),
                          argv0, cmdline);
        }
        else if (exit_code)
        {
            MsgToEventLog(M_ERR, TEXT("ExecCommand: \"%s %s\" exited with status = %lu"),
                          argv0, cmdline, exit_code);
        }
        else
        {
            MsgToEventLog(M_INFO, TEXT("ExecCommand: \"%s %s\" completed"), argv0, cmdline);
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        exit_code = GetLastError();
        MsgToEventLog(M_SYSERR, TEXT("ExecCommand: could not run \"%s %s\" :"),
                      argv0, cmdline);
    }

    free(cmdline_dup);
    return exit_code;
}
