static void *bzImage64_load(struct kimage *image, char *kernel,
			    unsigned long kernel_len, char *initrd,
			    unsigned long initrd_len, char *cmdline,
			    unsigned long cmdline_len)
{

	struct setup_header *header;
	int setup_sects, kern16_size, ret = 0;
	unsigned long setup_header_size, params_cmdline_sz, params_misc_sz;
	struct boot_params *params;
	unsigned long bootparam_load_addr, kernel_load_addr, initrd_load_addr;
	unsigned long purgatory_load_addr;
	unsigned long kernel_bufsz, kernel_memsz, kernel_align;
	char *kernel_buf;
	struct bzimage64_data *ldata;
	struct kexec_entry64_regs regs64;
	void *stack;
	unsigned int setup_hdr_offset = offsetof(struct boot_params, hdr);
	unsigned int efi_map_offset, efi_map_sz, efi_setup_data_offset;

	header = (struct setup_header *)(kernel + setup_hdr_offset);
	setup_sects = header->setup_sects;
	if (setup_sects == 0)
		setup_sects = 4;

	kern16_size = (setup_sects + 1) * 512;
	if (kernel_len < kern16_size) {
		pr_err("bzImage truncated\n");
		return ERR_PTR(-ENOEXEC);
	}

	if (cmdline_len > header->cmdline_size) {
		pr_err("Kernel command line too long\n");
		return ERR_PTR(-EINVAL);
	}

	/*
	 * In case of crash dump, we will append elfcorehdr=<addr> to
	 * command line. Make sure it does not overflow
	 */
	if (cmdline_len + MAX_ELFCOREHDR_STR_LEN > header->cmdline_size) {
		pr_debug("Appending elfcorehdr=<addr> to command line exceeds maximum allowed length\n");
		return ERR_PTR(-EINVAL);
	}

	/* Allocate and load backup region */
	if (image->type == KEXEC_TYPE_CRASH) {
		ret = crash_load_segments(image);
		if (ret)
			return ERR_PTR(ret);
	}

	/*
	 * Load purgatory. For 64bit entry point, purgatory  code can be
	 * anywhere.
	 */
	ret = kexec_load_purgatory(image, MIN_PURGATORY_ADDR, ULONG_MAX, 1,
				   &purgatory_load_addr);
	if (ret) {
		pr_err("Loading purgatory failed\n");
		return ERR_PTR(ret);
	}

	pr_debug("Loaded purgatory at 0x%lx\n", purgatory_load_addr);


	/*
	 * Load Bootparams and cmdline and space for efi stuff.
	 *
	 * Allocate memory together for multiple data structures so
	 * that they all can go in single area/segment and we don't
	 * have to create separate segment for each. Keeps things
	 * little bit simple
	 */
	efi_map_sz = efi_get_runtime_map_size();
	efi_map_sz = ALIGN(efi_map_sz, 16);
	params_cmdline_sz = sizeof(struct boot_params) + cmdline_len +
				MAX_ELFCOREHDR_STR_LEN;
	params_cmdline_sz = ALIGN(params_cmdline_sz, 16);
	params_misc_sz = params_cmdline_sz + efi_map_sz +
				sizeof(struct setup_data) +
				sizeof(struct efi_setup_data);

	params = kzalloc(params_misc_sz, GFP_KERNEL);
	if (!params)
		return ERR_PTR(-ENOMEM);
	efi_map_offset = params_cmdline_sz;
	efi_setup_data_offset = efi_map_offset + efi_map_sz;

	/* Copy setup header onto bootparams. Documentation/x86/boot.txt */
	setup_header_size = 0x0202 + kernel[0x0201] - setup_hdr_offset;

	/* Is there a limit on setup header size? */
	memcpy(&params->hdr, (kernel + setup_hdr_offset), setup_header_size);

	ret = kexec_add_buffer(image, (char *)params, params_misc_sz,
			       params_misc_sz, 16, MIN_BOOTPARAM_ADDR,
			       ULONG_MAX, 1, &bootparam_load_addr);
	if (ret)
		goto out_free_params;
	pr_debug("Loaded boot_param, command line and misc at 0x%lx bufsz=0x%lx memsz=0x%lx\n",
		 bootparam_load_addr, params_misc_sz, params_misc_sz);

	/* Load kernel */
	kernel_buf = kernel + kern16_size;
	kernel_bufsz =  kernel_len - kern16_size;
	kernel_memsz = PAGE_ALIGN(header->init_size);
	kernel_align = header->kernel_alignment;

	ret = kexec_add_buffer(image, kernel_buf,
			       kernel_bufsz, kernel_memsz, kernel_align,
			       MIN_KERNEL_LOAD_ADDR, ULONG_MAX, 1,
			       &kernel_load_addr);
	if (ret)
		goto out_free_params;

	pr_debug("Loaded 64bit kernel at 0x%lx bufsz=0x%lx memsz=0x%lx\n",
		 kernel_load_addr, kernel_memsz, kernel_memsz);

	/* Load initrd high */
	if (initrd) {
		ret = kexec_add_buffer(image, initrd, initrd_len, initrd_len,
				       PAGE_SIZE, MIN_INITRD_LOAD_ADDR,
				       ULONG_MAX, 1, &initrd_load_addr);
		if (ret)
			goto out_free_params;

		pr_debug("Loaded initrd at 0x%lx bufsz=0x%lx memsz=0x%lx\n",
				initrd_load_addr, initrd_len, initrd_len);

		setup_initrd(params, initrd_load_addr, initrd_len);
	}

	setup_cmdline(image, params, bootparam_load_addr,
		      sizeof(struct boot_params), cmdline, cmdline_len);

	/* bootloader info. Do we need a separate ID for kexec kernel loader? */
	params->hdr.type_of_loader = 0x0D << 4;
	params->hdr.loadflags = 0;

	/* Setup purgatory regs for entry */
	ret = kexec_purgatory_get_set_symbol(image, "entry64_regs", &regs64,
					     sizeof(regs64), 1);
	if (ret)
		goto out_free_params;

	regs64.rbx = 0; /* Bootstrap Processor */
	regs64.rsi = bootparam_load_addr;
	regs64.rip = kernel_load_addr + 0x200;
	stack = kexec_purgatory_get_symbol_addr(image, "stack_end");
	if (IS_ERR(stack)) {
		pr_err("Could not find address of symbol stack_end\n");
		ret = -EINVAL;
		goto out_free_params;
	}

	regs64.rsp = (unsigned long)stack;
	ret = kexec_purgatory_get_set_symbol(image, "entry64_regs", &regs64,
					     sizeof(regs64), 0);
	if (ret)
		goto out_free_params;

	ret = setup_boot_parameters(image, params, bootparam_load_addr,
				    efi_map_offset, efi_map_sz,
				    efi_setup_data_offset);
	if (ret)
		goto out_free_params;

	/* Allocate loader specific data */
	ldata = kzalloc(sizeof(struct bzimage64_data), GFP_KERNEL);
	if (!ldata) {
		ret = -ENOMEM;
		goto out_free_params;
	}

	/*
	 * Store pointer to params so that it could be freed after loading
	 * params segment has been loaded and contents have been copied
	 * somewhere else.
	 */
	ldata->bootparams_buf = params;
	return ldata;

out_free_params:
	kfree(params);
	return ERR_PTR(ret);
}
