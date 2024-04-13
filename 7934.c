static pj_status_t get_codec_info_from_sdp(pjmedia_endpt *endpt,
					   const pjmedia_sdp_media *m,
					   unsigned *sci_cnt,
					   sdp_codec_info_t sci[])
{
    pjmedia_codec_mgr *codec_mgr;
    unsigned j, cnt = 0;
    pjmedia_type type = PJMEDIA_TYPE_UNKNOWN;
    pj_status_t status;

    type = pjmedia_get_type(&m->desc.media);
    if (type != PJMEDIA_TYPE_AUDIO && type != PJMEDIA_TYPE_VIDEO)
	return PJMEDIA_EUNSUPMEDIATYPE;

    codec_mgr = pjmedia_endpt_get_codec_mgr(endpt);
    for (j = 0; j < m->desc.fmt_count && cnt < *sci_cnt; ++j) {
	unsigned pt = 0;
	pt = pj_strtoul(&m->desc.fmt[j]);
	if (pt < 96) {
	    if (type == PJMEDIA_TYPE_AUDIO) {
		const pjmedia_codec_info *ci;
		status = pjmedia_codec_mgr_get_codec_info(codec_mgr, pt, &ci);
		if (status != PJ_SUCCESS)
		    continue;

		pjmedia_codec_info_to_id(ci, sci[cnt].id, sizeof(sci[0].id));
	    } else {
#if defined(PJMEDIA_HAS_VIDEO) && (PJMEDIA_HAS_VIDEO != 0)
		const pjmedia_vid_codec_info *ci;
		status = pjmedia_vid_codec_mgr_get_codec_info(NULL, pt, &ci);
		if (status != PJ_SUCCESS)
		    continue;

		pjmedia_vid_codec_info_to_id(ci, sci[cnt].id,
					     sizeof(sci[0].id));
#else
		continue;
#endif
	    }
	} else {
	    pjmedia_sdp_attr *a;
	    pjmedia_sdp_rtpmap r;
	    a = pjmedia_sdp_media_find_attr2(m, "rtpmap",
					     &m->desc.fmt[j]);
	    if (a == NULL)
		continue;
	    status = pjmedia_sdp_attr_get_rtpmap(a, &r);
	    if (status != PJ_SUCCESS)
		continue;

	    if (type == PJMEDIA_TYPE_AUDIO) {
		/* Audio codec id format: "name/clock-rate/channel-count" */
		if (r.param.slen) {
		    pj_ansi_snprintf(sci[cnt].id, sizeof(sci[0].id),
				     "%.*s/%d/%.*s",
				     (int)r.enc_name.slen, r.enc_name.ptr,
				     r.clock_rate,
				     (int)r.param.slen, r.param.ptr);
		} else {
		    pj_ansi_snprintf(sci[cnt].id, sizeof(sci[0].id),
				     "%.*s/%d/1",
				     (int)r.enc_name.slen, r.enc_name.ptr,
				     r.clock_rate);
		}
	    } else {
		/* Video codec id format: "name/payload-type" */
		pj_ansi_snprintf(sci[cnt].id, sizeof(sci[0].id),
				 "%.*s/%d",
				 (int)r.enc_name.slen, r.enc_name.ptr, pt);
	    }
	}
	sci[cnt++].pt = pt;
    }
    *sci_cnt = cnt;
    
    return PJ_SUCCESS;
}