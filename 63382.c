error_detected(uint32_t errnum, char *errstr, ...)
{
    va_list args;

    va_start(args, errstr);

    {
        TSK_ERROR_INFO *errInfo = tsk_error_get_info();
        char *loc_errstr = errInfo->errstr;

        if (errInfo->t_errno == 0)
            errInfo->t_errno = errnum;
        else {
            size_t sl = strlen(errstr);
            snprintf(loc_errstr + sl, TSK_ERROR_STRING_MAX_LENGTH - sl,
                " Next errnum: 0x%x ", errnum);
        }
        if (errstr != NULL) {
            size_t sl = strlen(loc_errstr);
            vsnprintf(loc_errstr + sl, TSK_ERROR_STRING_MAX_LENGTH - sl,
                errstr, args);
        }
    }

    va_end(args);

}
