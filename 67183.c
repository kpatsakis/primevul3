R_API int r_bin_file_deref(RBin *bin, RBinFile *a) {
	RBinObject *o = r_bin_cur_object (bin);
	int res = false;
	if (a && !o) {
		res = true;
	} else if (a && o->referenced - 1 < 1) {
		res = true;
	} else if (o) {
		o->referenced--;
	}
	if (bin) {
		bin->cur = NULL;
	}
	return res;
}
