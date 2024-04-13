static void pjmedia_rtcp_fb_cap_dup(pj_pool_t *pool,
				    pjmedia_rtcp_fb_cap *dst,
				    const pjmedia_rtcp_fb_cap *src)
{
    pj_strdup(pool, &dst->codec_id, &src->codec_id);
    dst->type = src->type;
    pj_strdup(pool, &dst->type_name, &src->type_name);
    pj_strdup(pool, &dst->param, &src->param);
}