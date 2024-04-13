static bool bin_raw_strings(RCore *r, int mode, int va) {
	RBinFile *bf = r_bin_cur (r->bin);
	bool new_bf = false;
	if (bf && strstr (bf->file, "malloc://")) {
		ut8 *tmp = R_NEWS (ut8, bf->size);
		if (!tmp) {
			return false;
		}
		r_io_read_at (r->io, 0, tmp, bf->size);
		r_buf_write_at (bf->buf, 0, tmp, bf->size);
	}
	if (!r->file) {
		eprintf ("Core file not open\n");
		if (IS_MODE_JSON (mode)) {
			r_cons_print ("[]");
		}
		return false;
	}
	if (!bf) {
		bf = R_NEW0 (RBinFile);
		if (!bf) {
			return false;
		}
		RIODesc *desc = r_io_desc_get (r->io, r->file->fd);
		if (!desc) {
			free (bf);
			return false;
		}
		bf->file = strdup (desc->name);
		bf->size = r_io_desc_size (desc);
		if (bf->size == UT64_MAX) {
			free (bf);
			return false;
		}
		bf->buf = r_buf_new_with_io (&r->bin->iob, r->file->fd);
		bf->o = NULL;
		bf->rbin = r->bin;
		new_bf = true;
		va = false;
	}
	RList *l = r_bin_raw_strings (bf, 0);
	_print_strings (r, l, mode, va);
	r_list_free (l);
	if (new_bf) {
		r_buf_free (bf->buf);
		bf->buf = NULL;
		bf->id = -1;
		r_bin_file_free (bf);
	}
	return true;
}
