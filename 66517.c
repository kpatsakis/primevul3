static inline void add_dequant_dct(MpegEncContext *s,
                           int16_t *block, int i, uint8_t *dest, int line_size, int qscale)
{
    if (s->block_last_index[i] >= 0) {
        s->dct_unquantize_inter(s, block, i, qscale);

        s->idsp.idct_add(dest, line_size, block);
    }
}
