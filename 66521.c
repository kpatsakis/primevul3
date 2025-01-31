static void clear_context(MpegEncContext *s)
{
    int i, j, k;

    memset(&s->next_picture, 0, sizeof(s->next_picture));
    memset(&s->last_picture, 0, sizeof(s->last_picture));
    memset(&s->current_picture, 0, sizeof(s->current_picture));
    memset(&s->new_picture, 0, sizeof(s->new_picture));

    memset(s->thread_context, 0, sizeof(s->thread_context));

    s->me.map = NULL;
    s->me.score_map = NULL;
    s->dct_error_sum = NULL;
    s->block = NULL;
    s->blocks = NULL;
    s->block32 = NULL;
    memset(s->pblocks, 0, sizeof(s->pblocks));
    s->ac_val_base = NULL;
    s->ac_val[0] =
    s->ac_val[1] =
    s->ac_val[2] =NULL;
    s->sc.edge_emu_buffer = NULL;
    s->me.scratchpad = NULL;
    s->me.temp =
    s->sc.rd_scratchpad =
    s->sc.b_scratchpad =
    s->sc.obmc_scratchpad = NULL;


    s->bitstream_buffer = NULL;
    s->allocated_bitstream_buffer_size = 0;
    s->picture          = NULL;
    s->mb_type          = NULL;
    s->p_mv_table_base  = NULL;
    s->b_forw_mv_table_base = NULL;
    s->b_back_mv_table_base = NULL;
    s->b_bidir_forw_mv_table_base = NULL;
    s->b_bidir_back_mv_table_base = NULL;
    s->b_direct_mv_table_base = NULL;
    s->p_mv_table            = NULL;
    s->b_forw_mv_table       = NULL;
    s->b_back_mv_table       = NULL;
    s->b_bidir_forw_mv_table = NULL;
    s->b_bidir_back_mv_table = NULL;
    s->b_direct_mv_table     = NULL;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                s->b_field_mv_table_base[i][j][k] = NULL;
                s->b_field_mv_table[i][j][k] = NULL;
            }
            s->b_field_select_table[i][j] = NULL;
            s->p_field_mv_table_base[i][j] = NULL;
            s->p_field_mv_table[i][j] = NULL;
        }
        s->p_field_select_table[i] = NULL;
    }

    s->dc_val_base = NULL;
    s->coded_block_base = NULL;
    s->mbintra_table = NULL;
    s->cbp_table = NULL;
    s->pred_dir_table = NULL;

    s->mbskip_table = NULL;

    s->er.error_status_table = NULL;
    s->er.er_temp_buffer = NULL;
    s->mb_index2xy = NULL;
    s->lambda_table = NULL;

    s->cplx_tab = NULL;
    s->bits_tab = NULL;
}
