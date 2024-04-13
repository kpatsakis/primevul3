radeon_atombios_get_primary_dac_info(struct radeon_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, CompassionateData);
	uint16_t data_offset;
	struct _COMPASSIONATE_DATA *dac_info;
	uint8_t frev, crev;
	uint8_t bg, dac;
	struct radeon_encoder_primary_dac *p_dac = NULL;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {
		dac_info = (struct _COMPASSIONATE_DATA *)
			(mode_info->atom_context->bios + data_offset);

		p_dac = kzalloc(sizeof(struct radeon_encoder_primary_dac), GFP_KERNEL);

		if (!p_dac)
			return NULL;

		bg = dac_info->ucDAC1_BG_Adjustment;
		dac = dac_info->ucDAC1_DAC_Adjustment;
		p_dac->ps2_pdac_adj = (bg << 8) | (dac);

	}
	return p_dac;
}
