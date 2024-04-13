radeon_atombios_get_tv_dac_info(struct radeon_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, CompassionateData);
	uint16_t data_offset;
	struct _COMPASSIONATE_DATA *dac_info;
	uint8_t frev, crev;
	uint8_t bg, dac;
	struct radeon_encoder_tv_dac *tv_dac = NULL;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {

		dac_info = (struct _COMPASSIONATE_DATA *)
			(mode_info->atom_context->bios + data_offset);

		tv_dac = kzalloc(sizeof(struct radeon_encoder_tv_dac), GFP_KERNEL);

		if (!tv_dac)
			return NULL;

		bg = dac_info->ucDAC2_CRT2_BG_Adjustment;
		dac = dac_info->ucDAC2_CRT2_DAC_Adjustment;
		tv_dac->ps2_tvdac_adj = (bg << 16) | (dac << 20);

		bg = dac_info->ucDAC2_PAL_BG_Adjustment;
		dac = dac_info->ucDAC2_PAL_DAC_Adjustment;
		tv_dac->pal_tvdac_adj = (bg << 16) | (dac << 20);

		bg = dac_info->ucDAC2_NTSC_BG_Adjustment;
		dac = dac_info->ucDAC2_NTSC_DAC_Adjustment;
		tv_dac->ntsc_tvdac_adj = (bg << 16) | (dac << 20);

		tv_dac->tv_std = radeon_atombios_get_tv_info(rdev);
	}
	return tv_dac;
}
