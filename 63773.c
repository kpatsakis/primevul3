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
        pdftex_warn("%i bytes more than expected were ignored", l + 1);
        while (l-- > 0)
            t1_getbyte();
    }
}
