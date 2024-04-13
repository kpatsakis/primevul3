static void bin_mem_print(RList *mems, int perms, int depth, int mode) {
	RBinMem *mem;
	RListIter *iter;
	if (!mems) {
		return;
	}
	r_list_foreach (mems, iter, mem) {
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("{\"name\":\"%s\",\"size\":%d,\"address\":%d,"
					"\"flags\":\"%s\"}", mem->name, mem->size,
					mem->addr, r_str_rwx_i (mem->perms & perms));
		} else if (IS_MODE_SIMPLE (mode)) {
			r_cons_printf ("0x%08"PFMT64x"\n", mem->addr);
		} else {
			r_cons_printf ("0x%08"PFMT64x" +0x%04x %s %*s%-*s\n",
					mem->addr, mem->size, r_str_rwx_i (mem->perms & perms),
					depth, "", 20-depth, mem->name);
		}
		if (mem->mirrors) {
			if (IS_MODE_JSON (mode)) {
				r_cons_printf (",");
			}
			bin_mem_print (mem->mirrors, mem->perms & perms, depth + 1, mode);
		}
		if (IS_MODE_JSON (mode)) {
			if (iter->n) {
				r_cons_printf (",");
			}
		}
	}
}
