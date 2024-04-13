hfs_convert_2_unix_time(uint32_t hfsdate)
{
    if (hfsdate < NSEC_BTWN_1904_1970)
        return 0;
    return (uint32_t) (hfsdate - NSEC_BTWN_1904_1970);
}
