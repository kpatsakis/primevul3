get_model_name(__u8 family, __u8 model)
{
	static int overflow;
	char brand[128];
	int i;

	memcpy(brand, "Unknown", 8);
	if (ia64_pal_get_brand_info(brand)) {
		if (family == 0x7)
			memcpy(brand, "Merced", 7);
		else if (family == 0x1f) switch (model) {
			case 0: memcpy(brand, "McKinley", 9); break;
			case 1: memcpy(brand, "Madison", 8); break;
			case 2: memcpy(brand, "Madison up to 9M cache", 23); break;
		}
	}
	for (i = 0; i < MAX_BRANDS; i++)
		if (strcmp(brandname[i], brand) == 0)
			return brandname[i];
	for (i = 0; i < MAX_BRANDS; i++)
		if (brandname[i][0] == '\0')
			return strcpy(brandname[i], brand);
	if (overflow++ == 0)
		printk(KERN_ERR
		       "%s: Table overflow. Some processor model information will be missing\n",
		       __func__);
	return "Unknown";
}
