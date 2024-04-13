jas_iccprof_t *jas_iccprof_createfromclrspc(int clrspc)
{
	jas_iccprof_t *prof;
	switch (clrspc) {
	case JAS_CLRSPC_SRGB:
		prof = jas_iccprof_createfrombuf(jas_iccprofdata_srgb,
		  jas_iccprofdata_srgblen);
		break;
	case JAS_CLRSPC_SGRAY:
		prof = jas_iccprof_createfrombuf(jas_iccprofdata_sgray,
		  jas_iccprofdata_sgraylen);
		break;
	default:
		prof = 0;
		break;
	}
	return prof;
}
