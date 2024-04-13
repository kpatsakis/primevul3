static int validate_acl(FFServerStream *stream, HTTPContext *c)
{
    int ret = 0;
    FFServerIPAddressACL *acl;

    /* if stream->acl is null validate_acl_list will return 1 */
    ret = validate_acl_list(stream->acl, c);

    if (stream->dynamic_acl[0]) {
        acl = parse_dynamic_acl(stream, c);
        ret = validate_acl_list(acl, c);
        free_acl_list(acl);
    }

    return ret;
}
