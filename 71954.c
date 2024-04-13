bool am_is_paos_request(request_rec *r, int *error_code)
{
    const char *accept_header = NULL;
    const char *paos_header = NULL;
    bool have_paos_media_type = false;
    bool valid_paos_header = false;
    bool is_paos = false;
    ECPServiceOptions ecp_service_options = 0;

    *error_code = 0;
    accept_header = apr_table_get(r->headers_in, "Accept");
    paos_header = apr_table_get(r->headers_in, "PAOS");
    if (accept_header) {
        if (am_header_has_media_type(r, accept_header, MEDIA_TYPE_PAOS)) {
            have_paos_media_type = true;
        }
    }
    if (paos_header) {
        if (am_parse_paos_header(r, paos_header, &ecp_service_options)) {
            valid_paos_header = true;
        } else {
            if (*error_code == 0)
                *error_code = AM_ERROR_INVALID_PAOS_HEADER;
        }
    }
    if (have_paos_media_type) {
        if (valid_paos_header) {
            is_paos = true;
        } else {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "request supplied PAOS media type in Accept header "
                          "but omitted valid PAOS header");
            if (*error_code == 0)
                *error_code = AM_ERROR_MISSING_PAOS_HEADER;
        }
    } else {
        if (valid_paos_header) {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "request supplied valid PAOS header "
                          "but omitted PAOS media type in Accept header");
            if (*error_code == 0)
                *error_code = AM_ERROR_MISSING_PAOS_MEDIA_TYPE;
        }
    }
    AM_LOG_RERROR(APLOG_MARK, APLOG_DEBUG, 0, r,
                  "have_paos_media_type=%s valid_paos_header=%s is_paos=%s "
                  "error_code=%d ecp options=[%s]",
                  have_paos_media_type ? "True" : "False",
                  valid_paos_header ? "True" : "False",
                  is_paos ? "True" : "False",
                  *error_code,
                  am_ecp_service_options_str(r->pool, ecp_service_options));

    if (is_paos) {
        am_req_cfg_rec *req_cfg;

        req_cfg = am_get_req_cfg(r);
        req_cfg->ecp_service_options = ecp_service_options;
    }

    return is_paos;
}
