static int imapd_sasl_log(void *context __attribute__((unused)),
                          int level, const char *message)
{
    int syslog_level = LOG_INFO;

    switch (level) {
    case SASL_LOG_ERR:
    case SASL_LOG_FAIL:
        syslog_level = LOG_ERR;
        break;
    case SASL_LOG_WARN:
        syslog_level = LOG_WARNING;
        break;
    case SASL_LOG_DEBUG:
    case SASL_LOG_TRACE:
    case SASL_LOG_PASS:
        syslog_level = LOG_DEBUG;
        break;
    }

    syslog(syslog_level, "SASL %s", message);
    return SASL_OK;
}
