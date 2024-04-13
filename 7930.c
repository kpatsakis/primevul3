PJ_DEF(pj_status_t) pjmedia_rtcp_fb_decode_sdp2(
				    pj_pool_t *pool,
				    pjmedia_endpt *endpt,
				    const void *opt,
				    const pjmedia_sdp_session *sdp,
				    unsigned med_idx,
				    int pt,
				    pjmedia_rtcp_fb_info *info)
{
    unsigned sci_cnt = PJMEDIA_MAX_SDP_FMT;
    sdp_codec_info_t sci[PJMEDIA_MAX_SDP_FMT];
    const pjmedia_sdp_media *m;
    pj_status_t status;
    unsigned i;

    PJ_UNUSED_ARG(opt);

    PJ_ASSERT_RETURN(pool && endpt && opt==NULL && sdp, PJ_EINVAL);
    PJ_ASSERT_RETURN(med_idx < sdp->media_count, PJ_EINVAL);
    PJ_ASSERT_RETURN(pt <= 127, PJ_EINVAL);

    m = sdp->media[med_idx];
    status = get_codec_info_from_sdp(endpt, m, &sci_cnt, sci);
    if (status != PJ_SUCCESS)
	return status;

    pj_bzero(info, sizeof(*info));

    /* Iterate all SDP attribute a=rtcp-fb in the SDP media */
    for (i = 0; i < m->attr_count; ++i) {
	const pjmedia_sdp_attr *a = m->attr[i];
	pj_str_t token;
	pj_ssize_t tok_idx;
	unsigned j;
	const char *codec_id = NULL;
	pj_str_t type_name = {0};
	pjmedia_rtcp_fb_type type = PJMEDIA_RTCP_FB_OTHER;

	/* Skip non a=rtcp-fb */
	if (pj_strcmp2(&a->name, "rtcp-fb") != 0)
	    continue;

	/* Get PT */
	tok_idx = pj_strtok2(&a->value, " \t", &token, 0);
	if (tok_idx == a->value.slen)
	    continue;

	if (pj_strcmp2(&token, "*") == 0) {
	    /* All codecs */
	    codec_id = "*";
	} else {
	    /* Specific PT/codec */
	    unsigned pt_ = (unsigned) pj_strtoul2(&token, NULL, 10);
	    for (j = 0; j < sci_cnt; ++j) {
		/* Check if payload type is valid and requested */
		if (pt_ == sci[j].pt && (pt < 0 || pt == (int)pt_)) {
		    codec_id = sci[j].id;
		    break;
		}
	    }
	}

	/* Skip this a=rtcp-fb if PT is not recognized or not requested */
	if (!codec_id)
	    continue;

	/* Get RTCP-FB type */
	tok_idx = pj_strtok2(&a->value, " \t", &token, tok_idx + token.slen);
	if (tok_idx == a->value.slen)
	    continue;

	for (j = 0; j < PJ_ARRAY_SIZE(rtcp_fb_type_name); ++j) {
	    if (pj_strcmp2(&token, rtcp_fb_type_name[j].name) == 0) {
		type = rtcp_fb_type_name[j].type;
		break;
	    }
	}
	if (type == PJMEDIA_RTCP_FB_OTHER)
	    type_name = token;

	/* Got all the mandatory fields, let's initialize RTCP-FB cap */
	pj_strdup2(pool, &info->caps[info->cap_count].codec_id, codec_id);
	info->caps[info->cap_count].type = type;
	if (type == PJMEDIA_RTCP_FB_OTHER)
	    pj_strdup(pool, &info->caps[info->cap_count].type_name, &type_name);

	/* Get RTCP-FB param */
	tok_idx = pj_strtok2(&a->value, " \t", &token, tok_idx + token.slen);
	if (tok_idx != a->value.slen)
	    pj_strdup(pool, &info->caps[info->cap_count].param, &token);

	/* Next */
	if (++info->cap_count == PJMEDIA_RTCP_FB_MAX_CAP)
	    break;
    }

    return PJ_SUCCESS;
}