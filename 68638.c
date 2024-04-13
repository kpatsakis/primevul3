static int WM_GetOutput_Linear(midi * handle, int8_t *buffer, uint32_t size) {
    uint32_t buffer_used = 0;
    uint32_t i;
    struct _mdi *mdi = (struct _mdi *) handle;
    uint32_t real_samples_to_mix = 0;
    uint32_t data_pos;
    int32_t premix, left_mix, right_mix;
    struct _note *note_data = NULL;
    uint32_t count;
    struct _event *event = mdi->current_event;
    int32_t *tmp_buffer;
    int32_t *out_buffer;

    _WM_Lock(&mdi->lock);

    buffer_used = 0;
    memset(buffer, 0, size);

    if ( (size / 2) > mdi->mix_buffer_size) {
        if ( (size / 2) <= ( mdi->mix_buffer_size * 2 )) {
            mdi->mix_buffer_size += MEM_CHUNK;
        } else {
            mdi->mix_buffer_size = size / 2;
        }
        mdi->mix_buffer = realloc(mdi->mix_buffer, mdi->mix_buffer_size * sizeof(int32_t));
    }

    tmp_buffer = mdi->mix_buffer;

    memset(tmp_buffer, 0, ((size / 2) * sizeof(int32_t)));
    out_buffer = tmp_buffer;

    do {
        if (__builtin_expect((!mdi->samples_to_mix), 0)) {
            while ((!mdi->samples_to_mix) && (event->do_event)) {
                event->do_event(mdi, &event->event_data);
                if ((mdi->extra_info.mixer_options & WM_MO_LOOP) && (event[0].do_event == *_WM_do_meta_endoftrack)) {
                    _WM_ResetToStart(mdi);
                    event = mdi->current_event;
                } else {
                    mdi->samples_to_mix = event->samples_to_next;
                    event++;
                    mdi->current_event = event;
                }
            }

            if (__builtin_expect((!mdi->samples_to_mix), 0)) {
                if (mdi->extra_info.current_sample >= mdi->extra_info.approx_total_samples) {
                    break;
                } else if ((mdi->extra_info.approx_total_samples
                             - mdi->extra_info.current_sample) > (size >> 2)) {
                    mdi->samples_to_mix = size >> 2;
                } else {
                    mdi->samples_to_mix = mdi->extra_info.approx_total_samples
                                           - mdi->extra_info.current_sample;
                }
            }
        }
        if (__builtin_expect((mdi->samples_to_mix > (size >> 2)), 1)) {
            real_samples_to_mix = size >> 2;
        } else {
            real_samples_to_mix = mdi->samples_to_mix;
            if (real_samples_to_mix == 0) {
                continue;
            }
        }

        /* do mixing here */
        count = real_samples_to_mix;

        do {
            note_data = mdi->note;
            left_mix = right_mix = 0;
            RESAMPLE_DEBUGI("SAMPLES_TO_MIX",count);
            if (__builtin_expect((note_data != NULL), 1)) {
                RESAMPLE_DEBUGS("Processing Notes");
                while (note_data) {
                    /*
                     * ===================
                     * resample the sample
                     * ===================
                     */
                    data_pos = note_data->sample_pos >> FPBITS;
                    premix = ((note_data->sample->data[data_pos] + (((note_data->sample->data[data_pos + 1] - note_data->sample->data[data_pos]) * (int32_t)(note_data->sample_pos & FPMASK)) / 1024)) * (note_data->env_level >> 12)) / 1024;

                    left_mix += (premix * (int32_t)note_data->left_mix_volume) / 1024;
                    right_mix += (premix * (int32_t)note_data->right_mix_volume) / 1024;

                    /*
                     * ========================
                     * sample position checking
                     * ========================
                     */
#ifdef DEBUG_RESAMPLE
                    fprintf(stderr,"\r\n%d -> INC %i, ENV %i, LEVEL %i, TARGET %d, RATE %i, SAMPLE POS %i, SAMPLE LENGTH %i, PREMIX %i (%i:%i)",
                            (uint32_t)note_data,
                            note_data->env_inc,
                            note_data->env, note_data->env_level,
                            note_data->sample->env_target[note_data->env],
                            note_data->sample->env_rate[note_data->env],
                            note_data->sample_pos,
                            note_data->sample->data_length,
                            premix, left_mix, right_mix);
                    if (note_data->modes & SAMPLE_LOOP)
                        fprintf(stderr,", LOOP %i + %i",
                                note_data->sample->loop_start,
                                note_data->sample->loop_size);
                    fprintf(stderr,"\r\n");
#endif

                    note_data->sample_pos += note_data->sample_inc;

                    if (__builtin_expect((note_data->modes & SAMPLE_LOOP), 1)) {
                        if (__builtin_expect(
                                             (note_data->sample_pos > note_data->sample->loop_end),
                                             0)) {
                            note_data->sample_pos = note_data->sample->loop_start
                                + ((note_data->sample_pos
                                    - note_data->sample->loop_start)
                                % note_data->sample->loop_size);
                        }

                    } else if (__builtin_expect(
                                                  (note_data->sample_pos
                                                   >= note_data->sample->data_length),
                                                  0)) {
                        goto _END_THIS_NOTE;
                    }

                    if (__builtin_expect((note_data->env_inc == 0), 0)) {
                        note_data = note_data->next;
                        RESAMPLE_DEBUGS("Next Note: 0 env_inc");
                        continue;
                    }

                    note_data->env_level += note_data->env_inc;

                    if (note_data->env_inc < 0) {
                        if (__builtin_expect((note_data->env_level
                            > note_data->sample->env_target[note_data->env]), 0)) {
                            note_data = note_data->next;
                            RESAMPLE_DEBUGS("Next Note: env_lvl > env_target");
                            continue;
                        }
                    } else if (note_data->env_inc > 0) {
                        if (__builtin_expect((note_data->env_level
                            < note_data->sample->env_target[note_data->env]), 0)) {
                            note_data = note_data->next;
                            RESAMPLE_DEBUGS("Next Note: env_lvl < env_target");
                            continue;
                        }
                    }

                    note_data->env_level =
                            note_data->sample->env_target[note_data->env];
                    switch (note_data->env) {
                    case 0:
                        if (!(note_data->modes & SAMPLE_ENVELOPE)) {
                            note_data->env_inc = 0;
                            note_data = note_data->next;
                            RESAMPLE_DEBUGS("Next Note: No Envelope");
                            continue;
                        }
                        break;
                    case 2:
                        if (note_data->modes & SAMPLE_SUSTAIN /*|| note_data->hold*/) {
                            note_data->env_inc = 0;
                            note_data = note_data->next;
                            RESAMPLE_DEBUGS("Next Note: SAMPLE_SUSTAIN");
                            continue;
                        } else if (note_data->modes & SAMPLE_CLAMPED) {
                            note_data->env = 5;
                            if (note_data->env_level
                                    > note_data->sample->env_target[5]) {
                                note_data->env_inc =
                                        -note_data->sample->env_rate[5];
                            } else {
                                note_data->env_inc =
                                        note_data->sample->env_rate[5];
                            }
                            continue;
                        }
                        break;
                    case 5:
                        if (__builtin_expect((note_data->env_level == 0), 1)) {
                            goto _END_THIS_NOTE;
                        }
                        /* sample release */
                        if (note_data->modes & SAMPLE_LOOP)
                            note_data->modes ^= SAMPLE_LOOP;
                        note_data->env_inc = 0;
                        note_data = note_data->next;
                        RESAMPLE_DEBUGS("Next Note: Sample Release");

                        continue;
                    case 6:
                        _END_THIS_NOTE:
                        if (__builtin_expect((note_data->replay != NULL), 1)) {
                            note_data->active = 0;
                            {
                                struct _note *prev_note = NULL;
                                struct _note *nte_array = mdi->note;

                                if (nte_array != note_data) {
                                    do {
                                        prev_note = nte_array;
                                        nte_array = nte_array->next;
                                    } while (nte_array != note_data);
                                }
                                if (prev_note) {
                                    prev_note->next = note_data->replay;
                                } else {
                                    mdi->note = note_data->replay;
                                }
                                note_data->replay->next = note_data->next;
                                note_data = note_data->replay;
                                note_data->active = 1;
                            }
                        } else {
                            note_data->active = 0;
                            {
                                struct _note *prev_note = NULL;
                                struct _note *nte_array = mdi->note;

                                if (nte_array != note_data) {
                                    do {
                                        prev_note = nte_array;
                                        nte_array = nte_array->next;
                                    } while ((nte_array != note_data)
                                            && (nte_array));
                                }
                                if (prev_note) {
                                    prev_note->next = note_data->next;
                                } else {
                                    mdi->note = note_data->next;
                                }
                                note_data = note_data->next;
                            }
                        }
                        RESAMPLE_DEBUGS("Next Note: Killed Off Note");
                        continue;
                    }
                    note_data->env++;

                    if (note_data->is_off == 1) {
                        _WM_do_note_off_extra(note_data);
                    } else {

                        if (note_data->env_level
                            >= note_data->sample->env_target[note_data->env]) {
                            note_data->env_inc =
                                -note_data->sample->env_rate[note_data->env];
                        } else {
                            note_data->env_inc =
                                note_data->sample->env_rate[note_data->env];
                        }
                    }
                    note_data = note_data->next;
#ifdef DEBUG_RESAMPLE
                    if (note_data != NULL)
                        RESAMPLE_DEBUGI("Next Note: Next ENV ", note_data->env);
                    else
                        RESAMPLE_DEBUGS("Next Note: Next ENV");
#endif
                    continue;
                }
            }
            *tmp_buffer++ = left_mix;
            *tmp_buffer++ = right_mix;
        } while (--count);

        buffer_used += real_samples_to_mix * 4;
        size -= (real_samples_to_mix << 2);
        mdi->extra_info.current_sample += real_samples_to_mix;
        mdi->samples_to_mix -= real_samples_to_mix;
    } while (size);

    tmp_buffer = out_buffer;

    if (mdi->extra_info.mixer_options & WM_MO_REVERB) {
        _WM_do_reverb(mdi->reverb, tmp_buffer, (buffer_used / 2));
    }


    for (i = 0; i < buffer_used; i += 4) {
        left_mix = *tmp_buffer++;
        right_mix = *tmp_buffer++;

        /*
         * ===================
         * Write to the buffer
         * ===================
         */
#ifdef WORDS_BIGENDIAN
        (*buffer++) = ((left_mix >> 8) & 0x7f) | ((left_mix >> 24) & 0x80);
        (*buffer++) = left_mix & 0xff;
        (*buffer++) = ((right_mix >> 8) & 0x7f) | ((right_mix >> 24) & 0x80);
        (*buffer++) = right_mix & 0xff;
#else
        (*buffer++) = left_mix & 0xff;
        (*buffer++) = ((left_mix >> 8) & 0x7f) | ((left_mix >> 24) & 0x80);
        (*buffer++) = right_mix & 0xff;
        (*buffer++) = ((right_mix >> 8) & 0x7f) | ((right_mix >> 24) & 0x80);
#endif
    }

    _WM_Unlock(&mdi->lock);
    return (buffer_used);
}
