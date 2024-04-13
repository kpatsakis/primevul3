static void free_context_frame(MpegEncContext *s)
{
    int i, j, k;

    av_freep(&s->mb_type);
    av_freep(&s->p_mv_table_base);
    av_freep(&s->b_forw_mv_table_base);
    av_freep(&s->b_back_mv_table_base);
    av_freep(&s->b_bidir_forw_mv_table_base);
    av_freep(&s->b_bidir_back_mv_table_base);
    av_freep(&s->b_direct_mv_table_base);
    s->p_mv_table            = NULL;
    s->b_forw_mv_table       = NULL;
    s->b_back_mv_table       = NULL;
    s->b_bidir_forw_mv_table = NULL;
    s->b_bidir_back_mv_table = NULL;
    s->b_direct_mv_table     = NULL;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                av_freep(&s->b_field_mv_table_base[i][j][k]);
                s->b_field_mv_table[i][j][k] = NULL;
            }
            av_freep(&s->b_field_select_table[i][j]);
            av_freep(&s->p_field_mv_table_base[i][j]);
            s->p_field_mv_table[i][j] = NULL;
        }
        av_freep(&s->p_field_select_table[i]);
    }

    av_freep(&s->dc_val_base);
    av_freep(&s->coded_block_base);
    av_freep(&s->mbintra_table);
    av_freep(&s->cbp_table);
    av_freep(&s->pred_dir_table);

    av_freep(&s->mbskip_table);

    av_freep(&s->er.error_status_table);
    av_freep(&s->er.er_temp_buffer);
    av_freep(&s->mb_index2xy);
    av_freep(&s->lambda_table);

    av_freep(&s->cplx_tab);
    av_freep(&s->bits_tab);

    s->linesize = s->uvlinesize = 0;
}
