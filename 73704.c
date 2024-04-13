static struct radeon_hpd radeon_atom_get_hpd_info_from_gpio(struct radeon_device *rdev,
							    struct radeon_gpio_rec *gpio)
{
	struct radeon_hpd hpd;
	u32 reg;

	if (ASIC_IS_DCE4(rdev))
		reg = EVERGREEN_DC_GPIO_HPD_A;
	else
		reg = AVIVO_DC_GPIO_HPD_A;

	hpd.gpio = *gpio;
	if (gpio->reg == reg) {
		switch(gpio->mask) {
		case (1 << 0):
			hpd.hpd = RADEON_HPD_1;
			break;
		case (1 << 8):
			hpd.hpd = RADEON_HPD_2;
			break;
		case (1 << 16):
			hpd.hpd = RADEON_HPD_3;
			break;
		case (1 << 24):
			hpd.hpd = RADEON_HPD_4;
			break;
		case (1 << 26):
			hpd.hpd = RADEON_HPD_5;
			break;
		case (1 << 28):
			hpd.hpd = RADEON_HPD_6;
			break;
		default:
			hpd.hpd = RADEON_HPD_NONE;
			break;
		}
	} else
		hpd.hpd = RADEON_HPD_NONE;
	return hpd;
}
