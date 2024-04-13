int am_get_boolean_query_parameter(request_rec *r, const char *name,
                                   int *return_value, int default_value)
{
    char *value_str;
    int ret = OK;

    *return_value = default_value;

    value_str = am_extract_query_parameter(r->pool, r->args, name);
    if (value_str != NULL) {
        ret = am_urldecode(value_str);
        if (ret != OK) {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "Error urldecoding \"%s\" boolean query parameter, "
                          "value=\"%s\"", name, value_str);
            return ret;
        }
        if(!strcmp(value_str, "true")) {
            *return_value = TRUE;
        } else if(!strcmp(value_str, "false")) {
            *return_value = FALSE;
        } else {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "Invalid value for \"%s\" boolean query parameter, "
                          "value=\"%s\"", name, value_str);
            ret = HTTP_BAD_REQUEST;
        }
    }

    return ret;
}
