PJ_DEF(pj_status_t) pjmedia_rtcp_fb_parse_pli(
					const void *buf,
					pj_size_t length)
{
    pjmedia_rtcp_fb_common *hdr = (pjmedia_rtcp_fb_common*) buf;

    PJ_ASSERT_RETURN(buf, PJ_EINVAL);

    if (length < 12)
    	return PJ_ETOOSMALL;

    /* PLI uses pt==RTCP_PSFB and FMT==1 */
    if (hdr->rtcp_common.pt != RTCP_PSFB || hdr->rtcp_common.count != 1)
	return PJ_ENOTFOUND;

    return PJ_SUCCESS;
}