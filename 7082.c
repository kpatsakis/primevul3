static void cfg_handle_deprecated(cfg_t *cfg, cfg_opt_t *opt)
{
	if (is_set(CFGF_DROP, opt->flags)) {
		cfg_error(cfg, _("dropping deprecated configuration option '%s'"), opt->name);
		cfg_free_value(opt);
	} else {
		cfg_error(cfg, _("found deprecated option '%s', please update configuration file."), opt->name);
	}
}