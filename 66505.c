av_cold void ff_init_scantable(uint8_t *permutation, ScanTable *st,
                               const uint8_t *src_scantable)
{
    int i, end;

    st->scantable = src_scantable;

    for (i = 0; i < 64; i++) {
        int j = src_scantable[i];
        st->permutated[i] = permutation[j];
    }

    end = -1;
    for (i = 0; i < 64; i++) {
        int j = st->permutated[i];
        if (j > end)
            end = j;
        st->raster_end[i] = end;
    }
}
