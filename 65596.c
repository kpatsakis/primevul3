static void guess_mv(ERContext *s)
{
    int (*blocklist)[2], (*next_blocklist)[2];
    uint8_t *fixed;
    const ptrdiff_t mb_stride = s->mb_stride;
    const int mb_width  = s->mb_width;
    int mb_height = s->mb_height;
    int i, depth, num_avail;
    int mb_x, mb_y;
    ptrdiff_t mot_step, mot_stride;
    int blocklist_length, next_blocklist_length;

    if (s->last_pic.f && s->last_pic.f->data[0])
        mb_height = FFMIN(mb_height, (s->last_pic.f->height+15)>>4);
    if (s->next_pic.f && s->next_pic.f->data[0])
        mb_height = FFMIN(mb_height, (s->next_pic.f->height+15)>>4);

    blocklist      = (int (*)[2])s->er_temp_buffer;
    next_blocklist = blocklist + s->mb_stride * s->mb_height;
    fixed          = (uint8_t *)(next_blocklist + s->mb_stride * s->mb_height);

    set_mv_strides(s, &mot_step, &mot_stride);

    num_avail = 0;
    if (s->last_pic.motion_val[0])
        ff_thread_await_progress(s->last_pic.tf, mb_height-1, 0);
    for (i = 0; i < mb_width * mb_height; i++) {
        const int mb_xy = s->mb_index2xy[i];
        int f = 0;
        int error = s->error_status_table[mb_xy];

        if (IS_INTRA(s->cur_pic.mb_type[mb_xy]))
            f = MV_FROZEN; // intra // FIXME check
        if (!(error & ER_MV_ERROR))
            f = MV_FROZEN; // inter with undamaged MV

        fixed[mb_xy] = f;
        if (f == MV_FROZEN)
            num_avail++;
        else if(s->last_pic.f->data[0] && s->last_pic.motion_val[0]){
            const int mb_y= mb_xy / s->mb_stride;
            const int mb_x= mb_xy % s->mb_stride;
            const int mot_index= (mb_x + mb_y*mot_stride) * mot_step;
            s->cur_pic.motion_val[0][mot_index][0]= s->last_pic.motion_val[0][mot_index][0];
            s->cur_pic.motion_val[0][mot_index][1]= s->last_pic.motion_val[0][mot_index][1];
            s->cur_pic.ref_index[0][4*mb_xy]      = s->last_pic.ref_index[0][4*mb_xy];
        }
    }

    if ((!(s->avctx->error_concealment&FF_EC_GUESS_MVS)) ||
        num_avail <= mb_width / 2) {
        for (mb_y = 0; mb_y < mb_height; mb_y++) {
            for (mb_x = 0; mb_x < s->mb_width; mb_x++) {
                const int mb_xy = mb_x + mb_y * s->mb_stride;
                int mv_dir = (s->last_pic.f && s->last_pic.f->data[0]) ? MV_DIR_FORWARD : MV_DIR_BACKWARD;

                if (IS_INTRA(s->cur_pic.mb_type[mb_xy]))
                    continue;
                if (!(s->error_status_table[mb_xy] & ER_MV_ERROR))
                    continue;

                s->mv[0][0][0] = 0;
                s->mv[0][0][1] = 0;
                s->decode_mb(s->opaque, 0, mv_dir, MV_TYPE_16X16, &s->mv,
                             mb_x, mb_y, 0, 0);
            }
        }
        return;
    }

    blocklist_length = 0;
    for (mb_y = 0; mb_y < mb_height; mb_y++) {
        for (mb_x = 0; mb_x < mb_width; mb_x++) {
            const int mb_xy = mb_x + mb_y * mb_stride;
            if (fixed[mb_xy] == MV_FROZEN) {
                if (mb_x)               add_blocklist(blocklist, &blocklist_length, fixed, mb_x - 1, mb_y, mb_xy - 1);
                if (mb_y)               add_blocklist(blocklist, &blocklist_length, fixed, mb_x, mb_y - 1, mb_xy - mb_stride);
                if (mb_x+1 < mb_width)  add_blocklist(blocklist, &blocklist_length, fixed, mb_x + 1, mb_y, mb_xy + 1);
                if (mb_y+1 < mb_height) add_blocklist(blocklist, &blocklist_length, fixed, mb_x, mb_y + 1, mb_xy + mb_stride);
            }
        }
    }

    for (depth = 0; ; depth++) {
        int changed, pass, none_left;
        int blocklist_index;

        none_left = 1;
        changed   = 1;
        for (pass = 0; (changed || pass < 2) && pass < 10; pass++) {
            int score_sum = 0;

            changed = 0;
            for (blocklist_index = 0; blocklist_index < blocklist_length; blocklist_index++) {
                const int mb_x = blocklist[blocklist_index][0];
                const int mb_y = blocklist[blocklist_index][1];
                const int mb_xy = mb_x + mb_y * mb_stride;
                int mv_predictor[8][2];
                int ref[8];
                int pred_count;
                int j;
                int best_score;
                int best_pred;
                int mot_index;
                int prev_x, prev_y, prev_ref;

                if ((mb_x ^ mb_y ^ pass) & 1)
                    continue;
                av_assert2(fixed[mb_xy] != MV_FROZEN);


                av_assert1(!IS_INTRA(s->cur_pic.mb_type[mb_xy]));
                av_assert1(s->last_pic.f && s->last_pic.f->data[0]);

                j = 0;
                if (mb_x > 0)
                    j |= fixed[mb_xy - 1];
                if (mb_x + 1 < mb_width)
                    j |= fixed[mb_xy + 1];
                if (mb_y > 0)
                    j |= fixed[mb_xy - mb_stride];
                if (mb_y + 1 < mb_height)
                    j |= fixed[mb_xy + mb_stride];

                av_assert2(j & MV_FROZEN);

                if (!(j & MV_CHANGED) && pass > 1)
                    continue;

                none_left = 0;
                pred_count = 0;
                mot_index  = (mb_x + mb_y * mot_stride) * mot_step;

                if (mb_x > 0 && fixed[mb_xy - 1] > 1) {
                    mv_predictor[pred_count][0] =
                        s->cur_pic.motion_val[0][mot_index - mot_step][0];
                    mv_predictor[pred_count][1] =
                        s->cur_pic.motion_val[0][mot_index - mot_step][1];
                    ref[pred_count] =
                        s->cur_pic.ref_index[0][4 * (mb_xy - 1)];
                    pred_count++;
                }
                if (mb_x + 1 < mb_width && fixed[mb_xy + 1] > 1) {
                    mv_predictor[pred_count][0] =
                        s->cur_pic.motion_val[0][mot_index + mot_step][0];
                    mv_predictor[pred_count][1] =
                        s->cur_pic.motion_val[0][mot_index + mot_step][1];
                    ref[pred_count] =
                        s->cur_pic.ref_index[0][4 * (mb_xy + 1)];
                    pred_count++;
                }
                if (mb_y > 0 && fixed[mb_xy - mb_stride] > 1) {
                    mv_predictor[pred_count][0] =
                        s->cur_pic.motion_val[0][mot_index - mot_stride * mot_step][0];
                    mv_predictor[pred_count][1] =
                        s->cur_pic.motion_val[0][mot_index - mot_stride * mot_step][1];
                    ref[pred_count] =
                        s->cur_pic.ref_index[0][4 * (mb_xy - s->mb_stride)];
                    pred_count++;
                }
                if (mb_y + 1<mb_height && fixed[mb_xy + mb_stride] > 1) {
                    mv_predictor[pred_count][0] =
                        s->cur_pic.motion_val[0][mot_index + mot_stride * mot_step][0];
                    mv_predictor[pred_count][1] =
                        s->cur_pic.motion_val[0][mot_index + mot_stride * mot_step][1];
                    ref[pred_count] =
                        s->cur_pic.ref_index[0][4 * (mb_xy + s->mb_stride)];
                    pred_count++;
                }
                if (pred_count == 0)
                    continue;

                if (pred_count > 1) {
                    int sum_x = 0, sum_y = 0, sum_r = 0;
                    int max_x, max_y, min_x, min_y, max_r, min_r;

                    for (j = 0; j < pred_count; j++) {
                        sum_x += mv_predictor[j][0];
                        sum_y += mv_predictor[j][1];
                        sum_r += ref[j];
                        if (j && ref[j] != ref[j - 1])
                            goto skip_mean_and_median;
                    }

                    /* mean */
                    mv_predictor[pred_count][0] = sum_x / j;
                    mv_predictor[pred_count][1] = sum_y / j;
                             ref[pred_count]    = sum_r / j;

                    /* median */
                    if (pred_count >= 3) {
                        min_y = min_x = min_r =  99999;
                        max_y = max_x = max_r = -99999;
                    } else {
                        min_x = min_y = max_x = max_y = min_r = max_r = 0;
                    }
                    for (j = 0; j < pred_count; j++) {
                        max_x = FFMAX(max_x, mv_predictor[j][0]);
                        max_y = FFMAX(max_y, mv_predictor[j][1]);
                        max_r = FFMAX(max_r, ref[j]);
                        min_x = FFMIN(min_x, mv_predictor[j][0]);
                        min_y = FFMIN(min_y, mv_predictor[j][1]);
                        min_r = FFMIN(min_r, ref[j]);
                    }
                    mv_predictor[pred_count + 1][0] = sum_x - max_x - min_x;
                    mv_predictor[pred_count + 1][1] = sum_y - max_y - min_y;
                             ref[pred_count + 1]    = sum_r - max_r - min_r;

                    if (pred_count == 4) {
                        mv_predictor[pred_count + 1][0] /= 2;
                        mv_predictor[pred_count + 1][1] /= 2;
                                 ref[pred_count + 1]    /= 2;
                    }
                    pred_count += 2;
                }

skip_mean_and_median:
                /* zero MV */
                mv_predictor[pred_count][0] =
                mv_predictor[pred_count][1] =
                         ref[pred_count]    = 0;
                pred_count++;

                prev_x   = s->cur_pic.motion_val[0][mot_index][0];
                prev_y   = s->cur_pic.motion_val[0][mot_index][1];
                prev_ref = s->cur_pic.ref_index[0][4 * mb_xy];

                /* last MV */
                mv_predictor[pred_count][0] = prev_x;
                mv_predictor[pred_count][1] = prev_y;
                         ref[pred_count]    = prev_ref;
                pred_count++;

                best_pred = 0;
                best_score = 256 * 256 * 256 * 64;
                for (j = 0; j < pred_count; j++) {
                    int *linesize = s->cur_pic.f->linesize;
                    int score = 0;
                    uint8_t *src = s->cur_pic.f->data[0] +
                                   mb_x * 16 + mb_y * 16 * linesize[0];

                    s->cur_pic.motion_val[0][mot_index][0] =
                        s->mv[0][0][0] = mv_predictor[j][0];
                    s->cur_pic.motion_val[0][mot_index][1] =
                        s->mv[0][0][1] = mv_predictor[j][1];

                    if (ref[j] < 0)
                        continue;

                    s->decode_mb(s->opaque, ref[j], MV_DIR_FORWARD,
                                 MV_TYPE_16X16, &s->mv, mb_x, mb_y, 0, 0);

                    if (mb_x > 0 && fixed[mb_xy - 1] > 1) {
                        int k;
                        for (k = 0; k < 16; k++)
                            score += FFABS(src[k * linesize[0] - 1] -
                                           src[k * linesize[0]]);
                    }
                    if (mb_x + 1 < mb_width && fixed[mb_xy + 1] > 1) {
                        int k;
                        for (k = 0; k < 16; k++)
                            score += FFABS(src[k * linesize[0] + 15] -
                                           src[k * linesize[0] + 16]);
                    }
                    if (mb_y > 0 && fixed[mb_xy - mb_stride] > 1) {
                        int k;
                        for (k = 0; k < 16; k++)
                            score += FFABS(src[k - linesize[0]] - src[k]);
                    }
                    if (mb_y + 1 < mb_height && fixed[mb_xy + mb_stride] > 1) {
                        int k;
                        for (k = 0; k < 16; k++)
                            score += FFABS(src[k + linesize[0] * 15] -
                                           src[k + linesize[0] * 16]);
                    }

                    if (score <= best_score) { // <= will favor the last MV
                        best_score = score;
                        best_pred  = j;
                    }
                }
                score_sum += best_score;
                s->mv[0][0][0] = mv_predictor[best_pred][0];
                s->mv[0][0][1] = mv_predictor[best_pred][1];

                for (i = 0; i < mot_step; i++)
                    for (j = 0; j < mot_step; j++) {
                        s->cur_pic.motion_val[0][mot_index + i + j * mot_stride][0] = s->mv[0][0][0];
                        s->cur_pic.motion_val[0][mot_index + i + j * mot_stride][1] = s->mv[0][0][1];
                    }

                s->decode_mb(s->opaque, ref[best_pred], MV_DIR_FORWARD,
                             MV_TYPE_16X16, &s->mv, mb_x, mb_y, 0, 0);


                if (s->mv[0][0][0] != prev_x || s->mv[0][0][1] != prev_y) {
                    fixed[mb_xy] = MV_CHANGED;
                    changed++;
                } else
                    fixed[mb_xy] = MV_UNCHANGED;
            }
        }

        if (none_left)
            return;

        next_blocklist_length = 0;

        for (blocklist_index = 0; blocklist_index < blocklist_length; blocklist_index++) {
            const int mb_x = blocklist[blocklist_index][0];
            const int mb_y = blocklist[blocklist_index][1];
            const int mb_xy = mb_x + mb_y * mb_stride;

            if (fixed[mb_xy] & (MV_CHANGED|MV_UNCHANGED|MV_FROZEN)) {
                fixed[mb_xy] = MV_FROZEN;
                if (mb_x > 0)
                    add_blocklist(next_blocklist, &next_blocklist_length, fixed, mb_x - 1, mb_y, mb_xy - 1);
                if (mb_y > 0)
                    add_blocklist(next_blocklist, &next_blocklist_length, fixed, mb_x, mb_y - 1, mb_xy - mb_stride);
                if (mb_x + 1 < mb_width)
                    add_blocklist(next_blocklist, &next_blocklist_length, fixed, mb_x + 1, mb_y, mb_xy + 1);
                if (mb_y + 1 < mb_height)
                    add_blocklist(next_blocklist, &next_blocklist_length, fixed, mb_x, mb_y + 1, mb_xy + mb_stride);
            }
        }
        av_assert0(next_blocklist_length <= mb_height * mb_width);
        FFSWAP(int , blocklist_length, next_blocklist_length);
        FFSWAP(void*, blocklist, next_blocklist);
    }
}
