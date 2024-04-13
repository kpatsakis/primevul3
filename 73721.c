static inline struct radeon_i2c_bus_rec radeon_lookup_i2c_gpio(struct radeon_device *rdev,
							       uint8_t id)
{
	struct atom_context *ctx = rdev->mode_info.atom_context;
	ATOM_GPIO_I2C_ASSIGMENT *gpio;
	struct radeon_i2c_bus_rec i2c;
	int index = GetIndexIntoMasterTable(DATA, GPIO_I2C_Info);
	struct _ATOM_GPIO_I2C_INFO *i2c_info;
	uint16_t data_offset, size;
	int i, num_indices;

	memset(&i2c, 0, sizeof(struct radeon_i2c_bus_rec));
	i2c.valid = false;

	if (atom_parse_data_header(ctx, index, &size, NULL, NULL, &data_offset)) {
		i2c_info = (struct _ATOM_GPIO_I2C_INFO *)(ctx->bios + data_offset);

		num_indices = (size - sizeof(ATOM_COMMON_TABLE_HEADER)) /
			sizeof(ATOM_GPIO_I2C_ASSIGMENT);

		for (i = 0; i < num_indices; i++) {
			gpio = &i2c_info->asGPIO_Info[i];

			if (gpio->sucI2cId.ucAccess == id) {
				i2c.mask_clk_reg = le16_to_cpu(gpio->usClkMaskRegisterIndex) * 4;
				i2c.mask_data_reg = le16_to_cpu(gpio->usDataMaskRegisterIndex) * 4;
				i2c.en_clk_reg = le16_to_cpu(gpio->usClkEnRegisterIndex) * 4;
				i2c.en_data_reg = le16_to_cpu(gpio->usDataEnRegisterIndex) * 4;
				i2c.y_clk_reg = le16_to_cpu(gpio->usClkY_RegisterIndex) * 4;
				i2c.y_data_reg = le16_to_cpu(gpio->usDataY_RegisterIndex) * 4;
				i2c.a_clk_reg = le16_to_cpu(gpio->usClkA_RegisterIndex) * 4;
				i2c.a_data_reg = le16_to_cpu(gpio->usDataA_RegisterIndex) * 4;
				i2c.mask_clk_mask = (1 << gpio->ucClkMaskShift);
				i2c.mask_data_mask = (1 << gpio->ucDataMaskShift);
				i2c.en_clk_mask = (1 << gpio->ucClkEnShift);
				i2c.en_data_mask = (1 << gpio->ucDataEnShift);
				i2c.y_clk_mask = (1 << gpio->ucClkY_Shift);
				i2c.y_data_mask = (1 << gpio->ucDataY_Shift);
				i2c.a_clk_mask = (1 << gpio->ucClkA_Shift);
				i2c.a_data_mask = (1 << gpio->ucDataA_Shift);

				if (gpio->sucI2cId.sbfAccess.bfHW_Capable)
					i2c.hw_capable = true;
				else
					i2c.hw_capable = false;

				if (gpio->sucI2cId.ucAccess == 0xa0)
					i2c.mm_i2c = true;
				else
					i2c.mm_i2c = false;

				i2c.i2c_id = gpio->sucI2cId.ucAccess;

				i2c.valid = true;
				break;
			}
		}
	}

	return i2c;
}
