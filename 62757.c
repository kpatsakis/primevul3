static int decode_studiovisualobject(Mpeg4DecContext *ctx, GetBitContext *gb)
{
    MpegEncContext *s = &ctx->m;
    int visual_object_type;

        skip_bits(gb, 4); /* visual_object_verid */
        visual_object_type = get_bits(gb, 4);
        if (visual_object_type != VOT_VIDEO_ID) {
            avpriv_request_sample(s->avctx, "VO type %u", visual_object_type);
            return AVERROR_PATCHWELCOME;
        }

        next_start_code_studio(gb);
        extension_and_user_data(s, gb, 1);

    return 0;
}
