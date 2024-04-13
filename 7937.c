static pj_status_t add_sdp_attr_rtcp_fb( pj_pool_t *pool,
					 const char *pt,
					 const pjmedia_rtcp_fb_cap *cap,
					 pjmedia_sdp_media *m)
{
    pjmedia_sdp_attr *a;
    char tmp[128];
    pj_str_t val;
    pj_str_t type_name = {0};

    if (cap->type < PJMEDIA_RTCP_FB_OTHER)
	pj_cstr(&type_name, rtcp_fb_type_name[cap->type].name);
    else if (cap->type == PJMEDIA_RTCP_FB_OTHER)
	type_name = cap->type_name;

    if (type_name.slen == 0)
	return PJ_EINVAL;

    /* Generate RTCP FB param */
    if (cap->param.slen) {
	pj_ansi_snprintf(tmp, sizeof(tmp), "%s %.*s %.*s", pt,
			 (int)type_name.slen, type_name.ptr,
			 (int)cap->param.slen, cap->param.ptr);
    } else {
	pj_ansi_snprintf(tmp, sizeof(tmp), "%s %.*s", pt,
			 (int)type_name.slen, type_name.ptr);
    }
    pj_strset2(&val, tmp);

    /* Generate and add SDP attribute a=rtcp-fb */
    a = pjmedia_sdp_attr_create(pool, "rtcp-fb", &val);
    m->attr[m->attr_count++] = a;

    return PJ_SUCCESS;
}