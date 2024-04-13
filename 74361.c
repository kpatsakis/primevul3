static int bin_trycatch(RCore *core, int mode) {
	RBinFile *bf = r_bin_cur (core->bin);
	RListIter *iter;
	RBinTrycatch *tc;
	RList *trycatch = r_bin_file_get_trycatch (bf);
	int idx = 0;
	r_list_foreach (trycatch, iter, tc) {
		r_cons_printf ("f try.%d.%"PFMT64x".from=0x%08"PFMT64x"\n", idx, tc->source, tc->from);
		r_cons_printf ("f try.%d.%"PFMT64x".to=0x%08"PFMT64x"\n", idx, tc->source, tc->to);
		r_cons_printf ("f try.%d.%"PFMT64x".catch=0x%08"PFMT64x"\n", idx, tc->source, tc->handler);
		idx++;
	}
	return true;
}
