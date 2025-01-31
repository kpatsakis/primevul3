static int uvesafb_setup(char *options)
{
	char *this_opt;

	if (!options || !*options)
		return 0;

	while ((this_opt = strsep(&options, ",")) != NULL) {
		if (!*this_opt) continue;

		if (!strcmp(this_opt, "redraw"))
			ypan = 0;
		else if (!strcmp(this_opt, "ypan"))
			ypan = 1;
		else if (!strcmp(this_opt, "ywrap"))
			ypan = 2;
		else if (!strcmp(this_opt, "vgapal"))
			pmi_setpal = 0;
		else if (!strcmp(this_opt, "pmipal"))
			pmi_setpal = 1;
		else if (!strncmp(this_opt, "mtrr:", 5))
			mtrr = simple_strtoul(this_opt+5, NULL, 0);
		else if (!strcmp(this_opt, "nomtrr"))
			mtrr = 0;
		else if (!strcmp(this_opt, "nocrtc"))
			nocrtc = 1;
		else if (!strcmp(this_opt, "noedid"))
			noedid = 1;
		else if (!strcmp(this_opt, "noblank"))
			blank = 0;
		else if (!strncmp(this_opt, "vtotal:", 7))
			vram_total = simple_strtoul(this_opt + 7, NULL, 0);
		else if (!strncmp(this_opt, "vremap:", 7))
			vram_remap = simple_strtoul(this_opt + 7, NULL, 0);
		else if (!strncmp(this_opt, "maxhf:", 6))
			maxhf = simple_strtoul(this_opt + 6, NULL, 0);
		else if (!strncmp(this_opt, "maxvf:", 6))
			maxvf = simple_strtoul(this_opt + 6, NULL, 0);
		else if (!strncmp(this_opt, "maxclk:", 7))
			maxclk = simple_strtoul(this_opt + 7, NULL, 0);
		else if (!strncmp(this_opt, "vbemode:", 8))
			vbemode = simple_strtoul(this_opt + 8, NULL, 0);
		else if (this_opt[0] >= '0' && this_opt[0] <= '9') {
			mode_option = this_opt;
		} else {
			pr_warn("unrecognized option %s\n", this_opt);
		}
	}

	if (mtrr != 3 && mtrr != 0)
		pr_warn("uvesafb: mtrr should be set to 0 or 3; %d is unsupported", mtrr);

	return 0;
}
