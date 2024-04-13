bool radeon_atombios_sideport_present(struct radeon_device *rdev)
{
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, IntegratedSystemInfo);
	union igp_info *igp_info;
	u8 frev, crev;
	u16 data_offset;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {
		igp_info = (union igp_info *)(mode_info->atom_context->bios +
				      data_offset);
		switch (crev) {
		case 1:
			if (igp_info->info.ucMemoryType & 0xf0)
				return true;
			break;
		case 2:
			if (igp_info->info_2.ucMemoryType & 0x0f)
				return true;
			break;
		default:
			DRM_ERROR("Unsupported IGP table: %d %d\n", frev, crev);
			break;
		}
	}
	return false;
}
