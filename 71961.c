void am_set_cache_control_headers(request_rec *r)
{
    /* Send Cache-Control header to ensure that:
     * - no proxy in the path caches content inside this location (private),
     * - user agent have to revalidate content on server (must-revalidate).
     * - content is always stale as the session login status can change at any
     *   time synchronously (Redirect logout, session cookie is removed) or
     *   asynchronously (SOAP logout, session cookie still exists but is
     *   invalid),
     *
     * But never prohibit specifically any user agent to cache or store content
     *
     * Setting the headers in err_headers_out ensures that they will be
     * sent for all responses.
     */
    apr_table_setn(r->err_headers_out,
                   "Cache-Control", "private, max-age=0, must-revalidate");
}
