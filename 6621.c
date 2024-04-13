Bool gf_fs_check_filter_register_cap(const GF_FilterRegister *f_reg, u32 incode, GF_PropertyValue *cap_input, u32 outcode, GF_PropertyValue *cap_output, Bool exact_match_only)
{
	return gf_fs_check_filter_register_cap_ex(f_reg, incode, cap_input, outcode, cap_output, exact_match_only, GF_FALSE);
}