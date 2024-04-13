static void backup_duplicate_context(MpegEncContext *bak, MpegEncContext *src)
{
#define COPY(a) bak->a = src->a
    COPY(sc.edge_emu_buffer);
    COPY(me.scratchpad);
    COPY(me.temp);
    COPY(sc.rd_scratchpad);
    COPY(sc.b_scratchpad);
    COPY(sc.obmc_scratchpad);
    COPY(me.map);
    COPY(me.score_map);
    COPY(blocks);
    COPY(block);
    COPY(block32);
    COPY(start_mb_y);
    COPY(end_mb_y);
    COPY(me.map_generation);
    COPY(pb);
    COPY(dct_error_sum);
    COPY(dct_count[0]);
    COPY(dct_count[1]);
    COPY(ac_val_base);
    COPY(ac_val[0]);
    COPY(ac_val[1]);
    COPY(ac_val[2]);
#undef COPY
}
