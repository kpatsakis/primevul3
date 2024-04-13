void ff_mpeg_draw_horiz_band(MpegEncContext *s, int y, int h)
{
    ff_draw_horiz_band(s->avctx, s->current_picture_ptr->f,
                       s->last_picture_ptr ? s->last_picture_ptr->f : NULL, y, h, s->picture_structure,
                       s->first_field, s->low_delay);
}
