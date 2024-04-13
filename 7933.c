PJ_DEF(pj_status_t) pjmedia_rtcp_fb_decode_sdp(
				    pj_pool_t *pool,
				    pjmedia_endpt *endpt,
				    const void *opt,
				    const pjmedia_sdp_session *sdp,
				    unsigned med_idx,
				    pjmedia_rtcp_fb_info *info)
{
    return pjmedia_rtcp_fb_decode_sdp2(pool, endpt, opt, sdp, med_idx, -1,
				       info);
}