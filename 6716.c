static pgpDigParams pgpDigParamsNew(uint8_t tag)
{
    pgpDigParams digp = xcalloc(1, sizeof(*digp));
    digp->tag = tag;
    return digp;
}