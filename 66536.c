av_cold int ff_mpv_common_init(MpegEncContext *s)
{
    int i, ret;
    int nb_slices = (HAVE_THREADS &&
                     s->avctx->active_thread_type & FF_THREAD_SLICE) ?
                    s->avctx->thread_count : 1;

    clear_context(s);

    if (s->encoding && s->avctx->slices)
        nb_slices = s->avctx->slices;

    if (s->codec_id == AV_CODEC_ID_MPEG2VIDEO && !s->progressive_sequence)
        s->mb_height = (s->height + 31) / 32 * 2;
    else
        s->mb_height = (s->height + 15) / 16;

    if (s->avctx->pix_fmt == AV_PIX_FMT_NONE) {
        av_log(s->avctx, AV_LOG_ERROR,
               "decoding to AV_PIX_FMT_NONE is not supported.\n");
        return -1;
    }

    if (nb_slices > MAX_THREADS || (nb_slices > s->mb_height && s->mb_height)) {
        int max_slices;
        if (s->mb_height)
            max_slices = FFMIN(MAX_THREADS, s->mb_height);
        else
            max_slices = MAX_THREADS;
        av_log(s->avctx, AV_LOG_WARNING, "too many threads/slices (%d),"
               " reducing to %d\n", nb_slices, max_slices);
        nb_slices = max_slices;
    }

    if ((s->width || s->height) &&
        av_image_check_size(s->width, s->height, 0, s->avctx))
        return -1;

    dct_init(s);

    /* set chroma shifts */
    ret = av_pix_fmt_get_chroma_sub_sample(s->avctx->pix_fmt,
                                           &s->chroma_x_shift,
                                           &s->chroma_y_shift);
    if (ret)
        return ret;

    FF_ALLOCZ_OR_GOTO(s->avctx, s->picture,
                      MAX_PICTURE_COUNT * sizeof(Picture), fail);
    for (i = 0; i < MAX_PICTURE_COUNT; i++) {
        s->picture[i].f = av_frame_alloc();
        if (!s->picture[i].f)
            goto fail;
    }
    s->next_picture.f = av_frame_alloc();
    if (!s->next_picture.f)
        goto fail;
    s->last_picture.f = av_frame_alloc();
    if (!s->last_picture.f)
        goto fail;
    s->current_picture.f = av_frame_alloc();
    if (!s->current_picture.f)
        goto fail;
    s->new_picture.f = av_frame_alloc();
    if (!s->new_picture.f)
        goto fail;

    if (init_context_frame(s))
        goto fail;

    s->parse_context.state = -1;

    s->context_initialized = 1;
    memset(s->thread_context, 0, sizeof(s->thread_context));
    s->thread_context[0]   = s;

    if (nb_slices > 1) {
        for (i = 0; i < nb_slices; i++) {
            if (i) {
                s->thread_context[i] = av_memdup(s, sizeof(MpegEncContext));
                if (!s->thread_context[i])
                    goto fail;
            }
            if (init_duplicate_context(s->thread_context[i]) < 0)
                goto fail;
            s->thread_context[i]->start_mb_y =
                (s->mb_height * (i) + nb_slices / 2) / nb_slices;
            s->thread_context[i]->end_mb_y   =
                (s->mb_height * (i + 1) + nb_slices / 2) / nb_slices;
        }
    } else {
        if (init_duplicate_context(s) < 0)
            goto fail;
        s->start_mb_y = 0;
        s->end_mb_y   = s->mb_height;
    }
    s->slice_context_count = nb_slices;

    return 0;
 fail:
    ff_mpv_common_end(s);
    return -1;
}
