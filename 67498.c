ReturnLastError(HANDLE pipe, LPCWSTR func)
{
    ReturnError(pipe, GetLastError(), func, 1, &exit_event);
}
