static void uvesafb_init_info(struct fb_info *info, struct vbe_mode_ib *mode)
{
	unsigned int size_vmode;
	unsigned int size_remap;
	unsigned int size_total;
	struct uvesafb_par *par = info->par;
	int i, h;

	info->pseudo_palette = ((u8 *)info->par + sizeof(struct uvesafb_par));
	info->fix = uvesafb_fix;
	info->fix.ypanstep = par->ypan ? 1 : 0;
	info->fix.ywrapstep = (par->ypan > 1) ? 1 : 0;

	/* Disable blanking if the user requested so. */
	if (!blank)
		info->fbops->fb_blank = NULL;

	/*
	 * Find out how much IO memory is required for the mode with
	 * the highest resolution.
	 */
	size_remap = 0;
	for (i = 0; i < par->vbe_modes_cnt; i++) {
		h = par->vbe_modes[i].bytes_per_scan_line *
					par->vbe_modes[i].y_res;
		if (h > size_remap)
			size_remap = h;
	}
	size_remap *= 2;

	/*
	 *   size_vmode -- that is the amount of memory needed for the
	 *                 used video mode, i.e. the minimum amount of
	 *                 memory we need.
	 */
	size_vmode = info->var.yres * mode->bytes_per_scan_line;

	/*
	 *   size_total -- all video memory we have. Used for mtrr
	 *                 entries, resource allocation and bounds
	 *                 checking.
	 */
	size_total = par->vbe_ib.total_memory * 65536;
	if (vram_total)
		size_total = vram_total * 1024 * 1024;
	if (size_total < size_vmode)
		size_total = size_vmode;

	/*
	 *   size_remap -- the amount of video memory we are going to
	 *                 use for vesafb.  With modern cards it is no
	 *                 option to simply use size_total as th
	 *                 wastes plenty of kernel address space.
	 */
	if (vram_remap)
		size_remap = vram_remap * 1024 * 1024;
	if (size_remap < size_vmode)
		size_remap = size_vmode;
	if (size_remap > size_total)
		size_remap = size_total;

	info->fix.smem_len = size_remap;
	info->fix.smem_start = mode->phys_base_ptr;

	/*
	 * We have to set yres_virtual here because when setup_var() was
	 * called, smem_len wasn't defined yet.
	 */
	info->var.yres_virtual = info->fix.smem_len /
				 mode->bytes_per_scan_line;

	if (par->ypan && info->var.yres_virtual > info->var.yres) {
		pr_info("scrolling: %s using protected mode interface, yres_virtual=%d\n",
			(par->ypan > 1) ? "ywrap" : "ypan",
			info->var.yres_virtual);
	} else {
		pr_info("scrolling: redraw\n");
		info->var.yres_virtual = info->var.yres;
		par->ypan = 0;
	}

	info->flags = FBINFO_FLAG_DEFAULT |
			(par->ypan ? FBINFO_HWACCEL_YPAN : 0);

	if (!par->ypan)
		info->fbops->fb_pan_display = NULL;
}
