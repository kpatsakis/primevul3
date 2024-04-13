GF_Err gf_fs_set_separators(GF_FilterSession *session, const char *separator_set)
{
	if (!session) return GF_BAD_PARAM;
	if (separator_set && (strlen(separator_set)<5)) return GF_BAD_PARAM;

	if (separator_set) {
		session->sep_args = separator_set[0];
		session->sep_name = separator_set[1];
		session->sep_frag = separator_set[2];
		session->sep_list = separator_set[3];
		session->sep_neg = separator_set[4];
	} else {
		session->sep_args = ':';
		session->sep_name = '=';
		session->sep_frag = '#';
		session->sep_list = ',';
		session->sep_neg = '!';
	}
	return GF_OK;
}