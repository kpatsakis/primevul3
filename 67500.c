ReturnProcessId(HANDLE pipe, DWORD pid, DWORD count, LPHANDLE events)
{
    const WCHAR msg[] = L"Process ID";
    WCHAR buf[22 + _countof(msg)]; /* 10 chars each for error and PID and 2 for line breaks */

    /*
     * Same format as error messages (3 line string) with error = 0 in
     * 0x%08x format, PID on line 2 and a description "Process ID" on line 3
     */
    swprintf(buf, _countof(buf), L"0x%08x\n0x%08x\n%s", 0, pid, msg);
    buf[_countof(buf) - 1] = '\0';

    WritePipeAsync(pipe, buf, (DWORD)(wcslen(buf) * 2), count, events);
}
