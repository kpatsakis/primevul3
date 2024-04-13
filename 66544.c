void ff_set_qscale(MpegEncContext * s, int qscale)
{
    if (qscale < 1)
        qscale = 1;
    else if (qscale > 31)
        qscale = 31;

    s->qscale = qscale;
    s->chroma_qscale= s->chroma_qscale_table[qscale];

    s->y_dc_scale= s->y_dc_scale_table[ qscale ];
    s->c_dc_scale= s->c_dc_scale_table[ s->chroma_qscale ];
}
