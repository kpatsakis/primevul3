static void decode_smpte_tc(Mpeg4DecContext *ctx, GetBitContext *gb)
{
    MpegEncContext *s = &ctx->m;

    skip_bits(gb, 16); /* Time_code[63..48] */
    check_marker(s->avctx, gb, "after Time_code[63..48]");
    skip_bits(gb, 16); /* Time_code[47..32] */
    check_marker(s->avctx, gb, "after Time_code[47..32]");
    skip_bits(gb, 16); /* Time_code[31..16] */
    check_marker(s->avctx, gb, "after Time_code[31..16]");
    skip_bits(gb, 16); /* Time_code[15..0] */
    check_marker(s->avctx, gb, "after Time_code[15..0]");
    skip_bits(gb, 4); /* reserved_bits */
}
