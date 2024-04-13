bool radeon_get_atom_connector_info_from_object_table(struct drm_device *dev)
{
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	struct atom_context *ctx = mode_info->atom_context;
	int index = GetIndexIntoMasterTable(DATA, Object_Header);
	u16 size, data_offset;
	u8 frev, crev;
	ATOM_CONNECTOR_OBJECT_TABLE *con_obj;
	ATOM_DISPLAY_OBJECT_PATH_TABLE *path_obj;
	ATOM_OBJECT_HEADER *obj_header;
	int i, j, path_size, device_support;
	int connector_type;
	u16 igp_lane_info, conn_id, connector_object_id;
	bool linkb;
	struct radeon_i2c_bus_rec ddc_bus;
	struct radeon_gpio_rec gpio;
	struct radeon_hpd hpd;

	if (!atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset))
		return false;

	if (crev < 2)
		return false;

	obj_header = (ATOM_OBJECT_HEADER *) (ctx->bios + data_offset);
	path_obj = (ATOM_DISPLAY_OBJECT_PATH_TABLE *)
	    (ctx->bios + data_offset +
	     le16_to_cpu(obj_header->usDisplayPathTableOffset));
	con_obj = (ATOM_CONNECTOR_OBJECT_TABLE *)
	    (ctx->bios + data_offset +
	     le16_to_cpu(obj_header->usConnectorObjectTableOffset));
	device_support = le16_to_cpu(obj_header->usDeviceSupport);

	path_size = 0;
	for (i = 0; i < path_obj->ucNumOfDispPath; i++) {
		uint8_t *addr = (uint8_t *) path_obj->asDispPath;
		ATOM_DISPLAY_OBJECT_PATH *path;
		addr += path_size;
		path = (ATOM_DISPLAY_OBJECT_PATH *) addr;
		path_size += le16_to_cpu(path->usSize);
		linkb = false;
		if (device_support & le16_to_cpu(path->usDeviceTag)) {
			uint8_t con_obj_id, con_obj_num, con_obj_type;

			con_obj_id =
			    (le16_to_cpu(path->usConnObjectId) & OBJECT_ID_MASK)
			    >> OBJECT_ID_SHIFT;
			con_obj_num =
			    (le16_to_cpu(path->usConnObjectId) & ENUM_ID_MASK)
			    >> ENUM_ID_SHIFT;
			con_obj_type =
			    (le16_to_cpu(path->usConnObjectId) &
			     OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

			/* TODO CV support */
			if (le16_to_cpu(path->usDeviceTag) ==
				ATOM_DEVICE_CV_SUPPORT)
				continue;

			/* IGP chips */
			if ((rdev->flags & RADEON_IS_IGP) &&
			    (con_obj_id ==
			     CONNECTOR_OBJECT_ID_PCIE_CONNECTOR)) {
				uint16_t igp_offset = 0;
				ATOM_INTEGRATED_SYSTEM_INFO_V2 *igp_obj;

				index =
				    GetIndexIntoMasterTable(DATA,
							    IntegratedSystemInfo);

				if (atom_parse_data_header(ctx, index, &size, &frev,
							   &crev, &igp_offset)) {

					if (crev >= 2) {
						igp_obj =
							(ATOM_INTEGRATED_SYSTEM_INFO_V2
							 *) (ctx->bios + igp_offset);

						if (igp_obj) {
							uint32_t slot_config, ct;

							if (con_obj_num == 1)
								slot_config =
									igp_obj->
									ulDDISlot1Config;
							else
								slot_config =
									igp_obj->
									ulDDISlot2Config;

							ct = (slot_config >> 16) & 0xff;
							connector_type =
								object_connector_convert
								[ct];
							connector_object_id = ct;
							igp_lane_info =
								slot_config & 0xffff;
						} else
							continue;
					} else
						continue;
				} else {
					igp_lane_info = 0;
					connector_type =
						object_connector_convert[con_obj_id];
					connector_object_id = con_obj_id;
				}
			} else {
				igp_lane_info = 0;
				connector_type =
				    object_connector_convert[con_obj_id];
				connector_object_id = con_obj_id;
			}

			if (connector_type == DRM_MODE_CONNECTOR_Unknown)
				continue;

			for (j = 0; j < ((le16_to_cpu(path->usSize) - 8) / 2);
			     j++) {
				uint8_t enc_obj_id, enc_obj_num, enc_obj_type;

				enc_obj_id =
				    (le16_to_cpu(path->usGraphicObjIds[j]) &
				     OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
				enc_obj_num =
				    (le16_to_cpu(path->usGraphicObjIds[j]) &
				     ENUM_ID_MASK) >> ENUM_ID_SHIFT;
				enc_obj_type =
				    (le16_to_cpu(path->usGraphicObjIds[j]) &
				     OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

				/* FIXME: add support for router objects */
				if (enc_obj_type == GRAPH_OBJECT_TYPE_ENCODER) {
					if (enc_obj_num == 2)
						linkb = true;
					else
						linkb = false;

					radeon_add_atom_encoder(dev,
								enc_obj_id,
								le16_to_cpu
								(path->
								 usDeviceTag));

				}
			}

			/* look up gpio for ddc, hpd */
			if ((le16_to_cpu(path->usDeviceTag) &
			     (ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT)) == 0) {
				for (j = 0; j < con_obj->ucNumberOfObjects; j++) {
					if (le16_to_cpu(path->usConnObjectId) ==
					    le16_to_cpu(con_obj->asObjects[j].
							usObjectID)) {
						ATOM_COMMON_RECORD_HEADER
						    *record =
						    (ATOM_COMMON_RECORD_HEADER
						     *)
						    (ctx->bios + data_offset +
						     le16_to_cpu(con_obj->
								 asObjects[j].
								 usRecordOffset));
						ATOM_I2C_RECORD *i2c_record;
						ATOM_HPD_INT_RECORD *hpd_record;
						ATOM_I2C_ID_CONFIG_ACCESS *i2c_config;
						hpd.hpd = RADEON_HPD_NONE;

						while (record->ucRecordType > 0
						       && record->
						       ucRecordType <=
						       ATOM_MAX_OBJECT_RECORD_NUMBER) {
							switch (record->ucRecordType) {
							case ATOM_I2C_RECORD_TYPE:
								i2c_record =
								    (ATOM_I2C_RECORD *)
									record;
								i2c_config =
									(ATOM_I2C_ID_CONFIG_ACCESS *)
									&i2c_record->sucI2cId;
								ddc_bus = radeon_lookup_i2c_gpio(rdev,
												 i2c_config->
												 ucAccess);
								break;
							case ATOM_HPD_INT_RECORD_TYPE:
								hpd_record =
									(ATOM_HPD_INT_RECORD *)
									record;
								gpio = radeon_lookup_gpio(rdev,
											  hpd_record->ucHPDIntGPIOID);
								hpd = radeon_atom_get_hpd_info_from_gpio(rdev, &gpio);
								hpd.plugged_state = hpd_record->ucPlugged_PinState;
								break;
							}
							record =
							    (ATOM_COMMON_RECORD_HEADER
							     *) ((char *)record
								 +
								 record->
								 ucRecordSize);
						}
						break;
					}
				}
			} else {
				hpd.hpd = RADEON_HPD_NONE;
				ddc_bus.valid = false;
			}

			/* needed for aux chan transactions */
			ddc_bus.hpd_id = hpd.hpd ? (hpd.hpd - 1) : 0;

			conn_id = le16_to_cpu(path->usConnObjectId);

			if (!radeon_atom_apply_quirks
			    (dev, le16_to_cpu(path->usDeviceTag), &connector_type,
			     &ddc_bus, &conn_id, &hpd))
				continue;

			radeon_add_atom_connector(dev,
						  conn_id,
						  le16_to_cpu(path->
							      usDeviceTag),
						  connector_type, &ddc_bus,
						  linkb, igp_lane_info,
						  connector_object_id,
						  &hpd);

		}
	}

	radeon_link_encoder_connector(dev);

	return true;
}
