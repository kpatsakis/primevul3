static inline const char *spectre_v2_module_string(void)
{
	return spectre_v2_bad_module ? " - vulnerable module loaded" : "";
}
