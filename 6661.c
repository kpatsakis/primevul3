static Bool gf_fsess_get_user_pass(void *usr_cbk, const char *site_url, char *usr_name, char *password)
{
	GF_Event evt;
	GF_FilterSession *fsess = (GF_FilterSession *)usr_cbk;
	evt.type = GF_EVENT_AUTHORIZATION;
	evt.auth.site_url = site_url;
	evt.auth.user = usr_name;
	evt.auth.password = password;
	return gf_fs_forward_gf_event(fsess, &evt, GF_FALSE, GF_FALSE);
}