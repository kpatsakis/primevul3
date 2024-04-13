error_returned(char *errstr, ...)
{
    va_list args;
    va_start(args, errstr);

    {
        TSK_ERROR_INFO *errInfo = tsk_error_get_info();
        char *loc_errstr2 = errInfo->errstr2;

        if (errInfo->t_errno == 0)
            errInfo->t_errno = TSK_ERR_AUX_GENERIC;
        if (errstr != NULL) {
            size_t sl = strlen(loc_errstr2);
            vsnprintf(loc_errstr2 + sl, TSK_ERROR_STRING_MAX_LENGTH - sl,
                errstr, args);
        }
    }
    va_end(args);
}
