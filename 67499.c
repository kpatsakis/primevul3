ReturnOpenvpnOutput(HANDLE pipe, HANDLE ovpn_output, DWORD count, LPHANDLE events)
{
    WCHAR *wide_output = NULL;
    CHAR output[512];
    DWORD size;

    ReadFile(ovpn_output, output, sizeof(output), &size, NULL);
    if (size == 0)
    {
        return;
    }

    wide_output = malloc((size) * sizeof(WCHAR));
    if (wide_output)
    {
        MultiByteToWideChar(CP_UTF8, 0, output, size, wide_output, size);
        wide_output[size - 1] = 0;
    }

    ReturnError(pipe, ERROR_OPENVPN_STARTUP, wide_output, count, events);
    free(wide_output);
}
