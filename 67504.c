ServiceStartInteractiveOwn(DWORD dwArgc, LPTSTR *lpszArgv)
{
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStartInteractive(dwArgc, lpszArgv);
}
