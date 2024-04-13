PJ_DEF(pj_status_t) pjmedia_rtcp_fb_encode_sdp(
				    pj_pool_t *pool,
				    pjmedia_endpt *endpt,
				    const pjmedia_rtcp_fb_setting *opt,
				    pjmedia_sdp_session *sdp_local,
				    unsigned med_idx,
				    const pjmedia_sdp_session *sdp_remote)
{
    pjmedia_sdp_media *m = sdp_local->media[med_idx];
    unsigned i;
    unsigned sci_cnt = 0;
    sdp_codec_info_t sci[PJMEDIA_MAX_SDP_FMT];
    pj_status_t status;

    PJ_UNUSED_ARG(sdp_remote);

    PJ_ASSERT_RETURN(pool && endpt && opt && sdp_local, PJ_EINVAL);
    PJ_ASSERT_RETURN(med_idx < sdp_local->media_count, PJ_EINVAL);

    /* Add RTCP Feedback profile (AVPF), if configured to */
    if (!opt->dont_use_avpf) {
	unsigned proto = pjmedia_sdp_transport_get_proto(&m->desc.transport);
	if (!PJMEDIA_TP_PROTO_HAS_FLAG(proto, PJMEDIA_TP_PROFILE_RTCP_FB)) {
	    pj_str_t new_tp;
	    pj_strdup_with_null(pool, &new_tp, &m->desc.transport);
	    new_tp.ptr[new_tp.slen++] = 'F';
	    m->desc.transport = new_tp;
	}
    }

    /* Add RTCP Feedback capability to SDP */
    for (i = 0; i < opt->cap_count; ++i) {
	unsigned j;

	/* All codecs */
	if (pj_strcmp2(&opt->caps[i].codec_id, "*") == 0) {
	    status = add_sdp_attr_rtcp_fb(pool, "*", &opt->caps[i], m);
	    if (status != PJ_SUCCESS) {
		PJ_PERROR(3, (THIS_FILE, status,
			  "Failed generating SDP a=rtcp-fb:*"));
	    }
	    continue;
	}

	/* Specific codec */
	if (sci_cnt == 0) {
	    sci_cnt = PJ_ARRAY_SIZE(sci);
	    status = get_codec_info_from_sdp(endpt, m, &sci_cnt, sci);
	    if (status != PJ_SUCCESS) {
		PJ_PERROR(3, (THIS_FILE, status,
			  "Failed populating codec info from SDP"));
		return status;
	    }
	}

	for (j = 0; j < sci_cnt; ++j) {
	    if (pj_strnicmp2(&opt->caps[i].codec_id, sci[j].id,
			     opt->caps[i].codec_id.slen) == 0)
	    {
		char tmp[4];
		snprintf(tmp, sizeof(tmp), "%d", sci[j].pt);
		status = add_sdp_attr_rtcp_fb(pool, tmp, &opt->caps[i], m);
		if (status != PJ_SUCCESS) {
		    PJ_PERROR(3, (THIS_FILE, status,
			      "Failed generating SDP a=rtcp-fb:%d (%s)",
			      sci[j].pt, opt->caps[i].codec_id.ptr));
		}
		break;
	    }
	}
	if (j == sci_cnt) {
	    /* Codec ID not found in SDP (perhaps better ignore this error
	     * as app may configure audio and video in single setting).
	     */
	    PJ_PERROR(6, (THIS_FILE, PJ_ENOTFOUND,
		      "Failed generating SDP a=rtcp-fb for %s",
		      opt->caps[i].codec_id.ptr));
	}
    }

    return PJ_SUCCESS;
}