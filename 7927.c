PJ_DEF(pj_status_t) pjmedia_rtcp_fb_build_pli(
					pjmedia_rtcp_session *session, 
					void *buf,
					pj_size_t *length)
{
    pjmedia_rtcp_fb_common *hdr;
    unsigned len;

    PJ_ASSERT_RETURN(session && buf && length, PJ_EINVAL);

    len = 12;
    if (len > *length)
	return PJ_ETOOSMALL;

    /* Build RTCP-FB PLI header */
    hdr = (pjmedia_rtcp_fb_common*)buf;
    pj_memcpy(hdr, &session->rtcp_fb_com, sizeof(*hdr));
    hdr->rtcp_common.pt = RTCP_PSFB;
    hdr->rtcp_common.count = 1; /* FMT = 1 */
    hdr->rtcp_common.length = pj_htons((pj_uint16_t)(len/4 - 1));

    /* Finally */
    *length = len;

    return PJ_SUCCESS;
}