int am_save_post(request_rec *r, const char **relay_state)
{
    am_mod_cfg_rec *mod_cfg;
    const char *content_type;
    const char *charset;
    const char *psf_id;
    char *psf_name;
    char *post_data;
    apr_size_t post_data_len;
    apr_size_t written;
    apr_file_t *psf;

    mod_cfg = am_get_mod_cfg(r->server);
    if (mod_cfg->post_dir == NULL) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "MellonPostReplay enabled but MellonPostDirectory not set "
                      "-- cannot save post data");
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    if (am_postdir_cleanup(r) != OK)
        return HTTP_INTERNAL_SERVER_ERROR;

    /* Check Content-Type */
    content_type = apr_table_get(r->headers_in, "Content-Type");
    if (content_type == NULL) {
        content_type = "urlencoded";
        charset = NULL; 
    } else {
        if (am_has_header(r, content_type, 
            "application/x-www-form-urlencoded")) {
            content_type = "urlencoded";

        } else if (am_has_header(r, content_type,
                   "multipart/form-data")) {
            content_type = "multipart";

        } else {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "Unknown POST Content-Type \"%s\"", content_type);
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        charset = am_get_header_attr(r, content_type, NULL, "charset");
    }     

    if ((psf_id = am_generate_id(r)) == NULL) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r, "cannot generate id");
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    psf_name = apr_psprintf(r->pool, "%s/%s", mod_cfg->post_dir, psf_id);

    if (apr_file_open(&psf, psf_name,
                      APR_WRITE|APR_CREATE|APR_BINARY, 
                      APR_FPROT_UREAD|APR_FPROT_UWRITE,
                      r->pool) != OK) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "cannot create POST session file");
        return HTTP_INTERNAL_SERVER_ERROR;
    } 

    if (am_read_post_data(r, &post_data, &post_data_len) != OK) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r, "cannot read POST data");
        (void)apr_file_close(psf);
        return HTTP_INTERNAL_SERVER_ERROR;
    } 

    if (post_data_len > mod_cfg->post_size) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "POST data size %" APR_SIZE_T_FMT 
                      " exceeds maximum %" APR_SIZE_T_FMT ". "
                      "Increase MellonPostSize directive.",
                      post_data_len, mod_cfg->post_size);
        (void)apr_file_close(psf);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    written = post_data_len;
    if ((apr_file_write(psf, post_data, &written) != OK) ||
        (written != post_data_len)) { 
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "cannot write to POST session file");
            (void)apr_file_close(psf);
            return HTTP_INTERNAL_SERVER_ERROR;
    } 
    
    if (apr_file_close(psf) != OK) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "cannot close POST session file");
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    if (charset != NULL)
        charset = apr_psprintf(r->pool, "&charset=%s", 
                               am_urlencode(r->pool, charset));
    else 
        charset = "";

    *relay_state = apr_psprintf(r->pool, 
                                "%srepost?id=%s&ReturnTo=%s&enctype=%s%s",
                                am_get_endpoint_url(r), psf_id,
                                am_urlencode(r->pool, *relay_state), 
                                content_type, charset);

    return OK;
}
