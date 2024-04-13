static struct radeon_atom_ss *radeon_atombios_get_ss_info(struct
							  radeon_encoder
							  *encoder,
							  int id)
{
	struct drm_device *dev = encoder->base.dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, PPLL_SS_Info);
	uint16_t data_offset;
	struct _ATOM_SPREAD_SPECTRUM_INFO *ss_info;
	uint8_t frev, crev;
	struct radeon_atom_ss *ss = NULL;
	int i;

	if (id > ATOM_MAX_SS_ENTRY)
		return NULL;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {
		ss_info =
			(struct _ATOM_SPREAD_SPECTRUM_INFO *)(mode_info->atom_context->bios + data_offset);

		ss =
		    kzalloc(sizeof(struct radeon_atom_ss), GFP_KERNEL);

		if (!ss)
			return NULL;

		for (i = 0; i < ATOM_MAX_SS_ENTRY; i++) {
			if (ss_info->asSS_Info[i].ucSS_Id == id) {
				ss->percentage =
					le16_to_cpu(ss_info->asSS_Info[i].usSpreadSpectrumPercentage);
				ss->type = ss_info->asSS_Info[i].ucSpreadSpectrumType;
				ss->step = ss_info->asSS_Info[i].ucSS_Step;
				ss->delay = ss_info->asSS_Info[i].ucSS_Delay;
				ss->range = ss_info->asSS_Info[i].ucSS_Range;
				ss->refdiv = ss_info->asSS_Info[i].ucRecommendedRef_Div;
				break;
			}
		}
	}
	return ss;
}
