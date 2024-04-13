void ff_mpv_frame_end(MpegEncContext *s)
{
    emms_c();

    if (s->current_picture.reference)
        ff_thread_report_progress(&s->current_picture_ptr->tf, INT_MAX, 0);
}
