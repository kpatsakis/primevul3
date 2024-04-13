bool ass_gaussian_blur(const BitmapEngine *engine, Bitmap *bm, double r2)
{
    BlurMethod blur;
    find_best_method(&blur, r2);

    int w = bm->w, h = bm->h;
    int offset = ((2 * (blur.prefilter + blur.filter) + 17) << blur.level) - 5;
    int end_w = ((w + offset) & ~((1 << blur.level) - 1)) - 4;
    int end_h = ((h + offset) & ~((1 << blur.level) - 1)) - 4;

    const int stripe_width = 1 << (engine->align_order - 1);
    int size = end_h * ((end_w + stripe_width - 1) & ~(stripe_width - 1));
    int16_t *tmp = ass_aligned_alloc(2 * stripe_width, 4 * size, false);
    if (!tmp)
        return false;

    engine->stripe_unpack(tmp, bm->buffer, bm->stride, w, h);
    int16_t *buf[2] = {tmp, tmp + size};
    int index = 0;

    for (int i = 0; i < blur.level; ++i) {
        engine->shrink_vert(buf[index ^ 1], buf[index], w, h);
        h = (h + 5) >> 1;
        index ^= 1;
    }
    for (int i = 0; i < blur.level; ++i) {
        engine->shrink_horz(buf[index ^ 1], buf[index], w, h);
        w = (w + 5) >> 1;
        index ^= 1;
    }
    if (blur.prefilter) {
        engine->pre_blur_horz[blur.prefilter - 1](buf[index ^ 1], buf[index], w, h);
        w += 2 * blur.prefilter;
        index ^= 1;
    }
    engine->main_blur_horz[blur.filter](buf[index ^ 1], buf[index], w, h, blur.coeff);
    w += 2 * blur.filter + 8;
    index ^= 1;
    for (int i = 0; i < blur.level; ++i) {
        engine->expand_horz(buf[index ^ 1], buf[index], w, h);
        w = 2 * w + 4;
        index ^= 1;
    }
    if (blur.prefilter) {
        engine->pre_blur_vert[blur.prefilter - 1](buf[index ^ 1], buf[index], w, h);
        h += 2 * blur.prefilter;
        index ^= 1;
    }
    engine->main_blur_vert[blur.filter](buf[index ^ 1], buf[index], w, h, blur.coeff);
    h += 2 * blur.filter + 8;
    index ^= 1;
    for (int i = 0; i < blur.level; ++i) {
        engine->expand_vert(buf[index ^ 1], buf[index], w, h);
        h = 2 * h + 4;
        index ^= 1;
    }
    assert(w == end_w && h == end_h);

    if (!realloc_bitmap(engine, bm, w, h)) {
        ass_aligned_free(tmp);
        return false;
    }
    offset = ((blur.prefilter + blur.filter + 8) << blur.level) - 4;
    bm->left -= offset;
    bm->top  -= offset;

    engine->stripe_pack(bm->buffer, bm->stride, buf[index], w, h);
    ass_aligned_free(tmp);
    return true;
}
