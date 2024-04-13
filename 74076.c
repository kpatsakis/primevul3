static void print_quota_used(struct protstream *o, const struct quota *q)
{
    int res;
    const char *sep = "";

    prot_putc('(', o);
    for (res = 0 ; res < QUOTA_NUMRESOURCES ; res++) {
        if (q->limits[res] >= 0) {
            prot_printf(o, "%s%s " QUOTA_T_FMT " " QUOTA_T_FMT,
                        sep, quota_names[res],
                        q->useds[res]/quota_units[res],
                        q->limits[res]);
            sep = " ";
        }
    }
    prot_putc(')', o);
}
