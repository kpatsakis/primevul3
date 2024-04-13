WM_SYMBOL int WildMidi_FastSeek(midi * handle, unsigned long int *sample_pos) {
    struct _mdi *mdi;
    struct _event *event;
    struct _note *note_data;

    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    if (handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (-1);
    }
    if (sample_pos == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL seek position pointer)", 0);
        return (-1);
    }

    mdi = (struct _mdi *) handle;
    _WM_Lock(&mdi->lock);
    event = mdi->current_event;

    /* make sure we havent asked for a positions beyond the end of the song. */
    if (*sample_pos > mdi->extra_info.approx_total_samples) {
        /* if so set the position to the end of the song */
        *sample_pos = mdi->extra_info.approx_total_samples;
    }

    /* was end of song requested and are we are there? */
    if (*sample_pos == mdi->extra_info.approx_total_samples) {
        /* yes */
        _WM_Unlock(&mdi->lock);
        return (0);
    }

    /* did we want to fast forward? */
    if (mdi->extra_info.current_sample > *sample_pos) {
        /* no - reset some stuff */
        event = mdi->events;
        _WM_ResetToStart(handle);
        mdi->extra_info.current_sample = 0;
        mdi->samples_to_mix = 0;
    }

    if ((mdi->extra_info.current_sample + mdi->samples_to_mix) > *sample_pos) {
        mdi->samples_to_mix = (mdi->extra_info.current_sample + mdi->samples_to_mix) - *sample_pos;
        mdi->extra_info.current_sample = *sample_pos;
    } else {
        mdi->extra_info.current_sample += mdi->samples_to_mix;
        mdi->samples_to_mix = 0;
        while ((!mdi->samples_to_mix) && (event->do_event)) {
            event->do_event(mdi, &event->event_data);
            mdi->samples_to_mix = event->samples_to_next;
                
            if ((mdi->extra_info.current_sample + mdi->samples_to_mix) > *sample_pos) {
                mdi->samples_to_mix = (mdi->extra_info.current_sample + mdi->samples_to_mix) - *sample_pos;
                mdi->extra_info.current_sample = *sample_pos;
            } else {
                mdi->extra_info.current_sample += mdi->samples_to_mix;
                mdi->samples_to_mix = 0;
            }
            event++;
        }
        mdi->current_event = event;
    }

    /*
     * Clear notes as this is a fast seek so we only care
     * about new notes.
     *
     * NOTE: This function is for performance only.
     * Might need a WildMidi_SlowSeek if we need better accuracy.
     */
    note_data = mdi->note;
    if (note_data) {
        do {
            note_data->active = 0;
            if (note_data->replay) {
                note_data->replay = NULL;
            }
            note_data = note_data->next;
        } while (note_data);
    }
    mdi->note = NULL;

    /* clear the reverb buffers since we not gonna be using them here */
    _WM_reset_reverb(mdi->reverb);

    _WM_Unlock(&mdi->lock);
    return (0);
}
