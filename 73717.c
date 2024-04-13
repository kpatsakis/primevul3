radeon_atombios_get_tv_info(struct radeon_device *rdev)
{
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, AnalogTV_Info);
	uint16_t data_offset;
	uint8_t frev, crev;
	struct _ATOM_ANALOG_TV_INFO *tv_info;
	enum radeon_tv_std tv_std = TV_STD_NTSC;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {

		tv_info = (struct _ATOM_ANALOG_TV_INFO *)
			(mode_info->atom_context->bios + data_offset);

		switch (tv_info->ucTV_BootUpDefaultStandard) {
		case ATOM_TV_NTSC:
			tv_std = TV_STD_NTSC;
			DRM_INFO("Default TV standard: NTSC\n");
			break;
		case ATOM_TV_NTSCJ:
			tv_std = TV_STD_NTSC_J;
			DRM_INFO("Default TV standard: NTSC-J\n");
			break;
		case ATOM_TV_PAL:
			tv_std = TV_STD_PAL;
			DRM_INFO("Default TV standard: PAL\n");
			break;
		case ATOM_TV_PALM:
			tv_std = TV_STD_PAL_M;
			DRM_INFO("Default TV standard: PAL-M\n");
			break;
		case ATOM_TV_PALN:
			tv_std = TV_STD_PAL_N;
			DRM_INFO("Default TV standard: PAL-N\n");
			break;
		case ATOM_TV_PALCN:
			tv_std = TV_STD_PAL_CN;
			DRM_INFO("Default TV standard: PAL-CN\n");
			break;
		case ATOM_TV_PAL60:
			tv_std = TV_STD_PAL_60;
			DRM_INFO("Default TV standard: PAL-60\n");
			break;
		case ATOM_TV_SECAM:
			tv_std = TV_STD_SECAM;
			DRM_INFO("Default TV standard: SECAM\n");
			break;
		default:
			tv_std = TV_STD_NTSC;
			DRM_INFO("Unknown TV standard; defaulting to NTSC\n");
			break;
		}
	}
	return tv_std;
}
