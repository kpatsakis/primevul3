find_ms_san(hx509_context context, hx509_cert cert, void *ctx)
{
    char *upn;
    int ret;

    ret = get_ms_san(context, cert, &upn);
    if (ret == 0)
	free(upn);
    return ret;
}
