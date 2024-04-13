static av_always_inline void add_blocklist(int (*blocklist)[2], int *blocklist_length, uint8_t *fixed, int mb_x, int mb_y, int mb_xy)
{
    if (fixed[mb_xy])
        return;
    fixed[mb_xy] = MV_LISTED;
    blocklist[ *blocklist_length   ][0] = mb_x;
    blocklist[(*blocklist_length)++][1] = mb_y;
}
