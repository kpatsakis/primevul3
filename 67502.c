ServiceCtrlInteractive(DWORD ctrl_code, DWORD event, LPVOID data, LPVOID ctx)
{
    SERVICE_STATUS *status = ctx;
    switch (ctrl_code)
    {
        case SERVICE_CONTROL_STOP:
            status->dwCurrentState = SERVICE_STOP_PENDING;
            ReportStatusToSCMgr(service, status);
            if (exit_event)
            {
                SetEvent(exit_event);
            }
            return NO_ERROR;

        case SERVICE_CONTROL_INTERROGATE:
            return NO_ERROR;

        default:
            return ERROR_CALL_NOT_IMPLEMENTED;
    }
}
