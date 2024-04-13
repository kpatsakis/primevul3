PJ_DEF(pj_status_t) pjmedia_rtcp_fb_setting_default(
					pjmedia_rtcp_fb_setting *opt)
{
    pj_bzero(opt, sizeof(*opt));
    opt->dont_use_avpf = PJ_TRUE;

    return PJ_SUCCESS;
}