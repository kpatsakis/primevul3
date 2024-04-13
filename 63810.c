static int t1_getbyte(void)
{
    int c = t1_getchar();
    if (t1_pfa)
        return c;
    if (t1_block_length == 0) {
        if (c != 128)
            normal_error("type 1","invalid marker");
        c = t1_getchar();
        if (c == 3) {
            while (!t1_eof())
                (void) t1_getchar();
            return EOF;
        }
        t1_block_length = t1_getchar() & 0xff;
        t1_block_length |= (t1_getchar() & 0xff) << 8;
        t1_block_length |= (t1_getchar() & 0xff) << 16;
        t1_block_length |= (t1_getchar() & 0xff) << 24;
        c = t1_getchar();
    }
    t1_block_length--;
    return c;
}
