GF_Err gf_sm_load_string(GF_SceneLoader *load, const char *str, Bool do_clean)
{
	GF_Err e;
	if (!load->type) e = GF_BAD_PARAM;
	else if (load->parse_string) e = load->parse_string(load, str);
	else e = GF_NOT_SUPPORTED;

	return e;
}
