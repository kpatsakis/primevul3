static Bool hevc_cleanup_config(GF_HEVCConfig *cfg, HevcConfigUpdateType operand_type)
{
	u32 i;
	Bool array_incomplete = (operand_type==GF_ISOM_HVCC_SET_INBAND) ? 1 : 0;
	if (!cfg) return 0;

	for (i=0; i<gf_list_count(cfg->param_array); i++) {
		GF_HEVCParamArray *ar = (GF_HEVCParamArray*)gf_list_get(cfg->param_array, i);

		/*we want to force hev1*/
		if (operand_type==GF_ISOM_HVCC_SET_INBAND) {
			ar->array_completeness = 0;

			while (gf_list_count(ar->nalus)) {
				GF_AVCConfigSlot *sl = (GF_AVCConfigSlot*)gf_list_get(ar->nalus, 0);
				gf_list_rem(ar->nalus, 0);
				if (sl->data) gf_free(sl->data);
				gf_free(sl);
			}
			gf_list_del(ar->nalus);
			gf_free(ar);
			gf_list_rem(cfg->param_array, i);
			i--;
		}
		if (!ar->array_completeness) 
			array_incomplete = 1;
	}
	return array_incomplete;
}
