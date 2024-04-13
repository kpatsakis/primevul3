static void t1_check_block_len(boolean decrypt)
{
    int l, c;
    if (t1_block_length == 0)
        return;
    c = t1_getbyte();
    if (decrypt)
        c = edecrypt((byte)c);
    l = t1_block_length;
    if (!(l == 0 && (c == 10 || c == 13))) {
        pdftex_fail("%i bytes more than expected", l + 1);
    }
}
