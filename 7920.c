PJ_DEF(void) pjmedia_rtcp_fb_info_dup( pj_pool_t *pool,
				       pjmedia_rtcp_fb_info *dst,
				       const pjmedia_rtcp_fb_info *src)
{
    unsigned i;

    pj_assert(pool && dst && src);

    pj_memcpy(dst, src, sizeof(pjmedia_rtcp_fb_info));
    for (i = 0; i < src->cap_count; ++i) {
	pjmedia_rtcp_fb_cap_dup(pool, &dst->caps[i], &src->caps[i]);
    }
}