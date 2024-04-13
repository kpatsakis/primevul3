struct radeon_encoder_atom_dig *radeon_atombios_get_lvds_info(struct
							      radeon_encoder
							      *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, LVDS_Info);
	uint16_t data_offset, misc;
	union lvds_info *lvds_info;
	uint8_t frev, crev;
	struct radeon_encoder_atom_dig *lvds = NULL;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {
		lvds_info =
			(union lvds_info *)(mode_info->atom_context->bios + data_offset);
		lvds =
		    kzalloc(sizeof(struct radeon_encoder_atom_dig), GFP_KERNEL);

		if (!lvds)
			return NULL;

		lvds->native_mode.clock =
		    le16_to_cpu(lvds_info->info.sLCDTiming.usPixClk) * 10;
		lvds->native_mode.hdisplay =
		    le16_to_cpu(lvds_info->info.sLCDTiming.usHActive);
		lvds->native_mode.vdisplay =
		    le16_to_cpu(lvds_info->info.sLCDTiming.usVActive);
		lvds->native_mode.htotal = lvds->native_mode.hdisplay +
			le16_to_cpu(lvds_info->info.sLCDTiming.usHBlanking_Time);
		lvds->native_mode.hsync_start = lvds->native_mode.hdisplay +
			le16_to_cpu(lvds_info->info.sLCDTiming.usHSyncOffset);
		lvds->native_mode.hsync_end = lvds->native_mode.hsync_start +
			le16_to_cpu(lvds_info->info.sLCDTiming.usHSyncWidth);
		lvds->native_mode.vtotal = lvds->native_mode.vdisplay +
			le16_to_cpu(lvds_info->info.sLCDTiming.usVBlanking_Time);
		lvds->native_mode.vsync_start = lvds->native_mode.vdisplay +
			le16_to_cpu(lvds_info->info.sLCDTiming.usVSyncWidth);
		lvds->native_mode.vsync_end = lvds->native_mode.vsync_start +
			le16_to_cpu(lvds_info->info.sLCDTiming.usVSyncWidth);
		lvds->panel_pwr_delay =
		    le16_to_cpu(lvds_info->info.usOffDelayInMs);
		lvds->lvds_misc = lvds_info->info.ucLVDS_Misc;

		misc = le16_to_cpu(lvds_info->info.sLCDTiming.susModeMiscInfo.usAccess);
		if (misc & ATOM_VSYNC_POLARITY)
			lvds->native_mode.flags |= DRM_MODE_FLAG_NVSYNC;
		if (misc & ATOM_HSYNC_POLARITY)
			lvds->native_mode.flags |= DRM_MODE_FLAG_NHSYNC;
		if (misc & ATOM_COMPOSITESYNC)
			lvds->native_mode.flags |= DRM_MODE_FLAG_CSYNC;
		if (misc & ATOM_INTERLACE)
			lvds->native_mode.flags |= DRM_MODE_FLAG_INTERLACE;
		if (misc & ATOM_DOUBLE_CLOCK_MODE)
			lvds->native_mode.flags |= DRM_MODE_FLAG_DBLSCAN;

		/* set crtc values */
		drm_mode_set_crtcinfo(&lvds->native_mode, CRTC_INTERLACE_HALVE_V);

		lvds->ss = radeon_atombios_get_ss_info(encoder, lvds_info->info.ucSS_Id);

		if (ASIC_IS_AVIVO(rdev)) {
			if (radeon_new_pll == 0)
				lvds->pll_algo = PLL_ALGO_LEGACY;
			else
				lvds->pll_algo = PLL_ALGO_NEW;
		} else {
			if (radeon_new_pll == 1)
				lvds->pll_algo = PLL_ALGO_NEW;
			else
				lvds->pll_algo = PLL_ALGO_LEGACY;
		}

		encoder->native_mode = lvds->native_mode;
	}
	return lvds;
}
