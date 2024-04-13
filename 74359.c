static int bin_source(RCore *r, int mode) {
	RList *final_list = r_list_new ();
	RBinFile * binfile = r->bin->cur;

	if (!binfile) {
		bprintf ("[Error bin file]\n");
		r_list_free (final_list);
		return false;
	}

	SdbListIter *iter;
	RListIter *iter2;
	char* srcline;
	SdbKv *kv;
	SdbList *ls = sdb_foreach_list (binfile->sdb_addrinfo, false);
	ls_foreach (ls, iter, kv) {
		char *v = sdbkv_value (kv);
		RList *list = r_str_split_list (v, "|", 0);
		srcline = r_list_get_bottom (list);
		if (srcline) {
			if (!strstr (srcline, "0x")){
				r_list_append (final_list, srcline);
			}
		}
		r_list_free (list);
	}
	r_cons_printf ("[Source file]\n");
	RList *uniqlist = r_list_uniq (final_list, srclineCmp);
	r_list_foreach (uniqlist, iter2, srcline) {
		r_cons_printf ("%s\n", srcline);
	}
	r_list_free (uniqlist);
	r_list_free (final_list);
	return true;
}
