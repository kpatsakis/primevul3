BlockDNSErrHandler(DWORD err, const char *msg)
{
    TCHAR buf[256];
    LPCTSTR err_str;

    if (!err)
    {
        return;
    }

    err_str = TEXT("Unknown Win32 Error");

    if (FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM
                      | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                      NULL, err, 0, buf, sizeof(buf), NULL))
    {
        err_str = buf;
    }

#ifdef UNICODE
    MsgToEventLog(M_ERR, L"%S (status = %lu): %s", msg, err, err_str);
#else
    MsgToEventLog(M_ERR, "%s (status = %lu): %s", msg, err, err_str);
#endif

}
