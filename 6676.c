void gf_fs_enable_reporting(GF_FilterSession *session, Bool reporting_on)
{
	if (session) session->reporting_on = reporting_on;
}