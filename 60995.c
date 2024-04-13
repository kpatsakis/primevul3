static int gen_pktline(git_buf *buf, git_push *push)
{
	push_spec *spec;
	size_t i, len;
	char old_id[GIT_OID_HEXSZ+1], new_id[GIT_OID_HEXSZ+1];

	old_id[GIT_OID_HEXSZ] = '\0'; new_id[GIT_OID_HEXSZ] = '\0';

	git_vector_foreach(&push->specs, i, spec) {
		len = 2*GIT_OID_HEXSZ + 7 + strlen(spec->refspec.dst);

		if (i == 0) {
			++len; /* '\0' */
			if (push->report_status)
				len += strlen(GIT_CAP_REPORT_STATUS) + 1;
			len += strlen(GIT_CAP_SIDE_BAND_64K) + 1;
		}

		git_oid_fmt(old_id, &spec->roid);
		git_oid_fmt(new_id, &spec->loid);

		git_buf_printf(buf, "%04"PRIxZ"%s %s %s", len, old_id, new_id, spec->refspec.dst);

		if (i == 0) {
			git_buf_putc(buf, '\0');
			/* Core git always starts their capabilities string with a space */
			if (push->report_status) {
				git_buf_putc(buf, ' ');
				git_buf_printf(buf, GIT_CAP_REPORT_STATUS);
			}
			git_buf_putc(buf, ' ');
			git_buf_printf(buf, GIT_CAP_SIDE_BAND_64K);
		}

		git_buf_putc(buf, '\n');
	}

	git_buf_puts(buf, "0000");
	return git_buf_oom(buf) ? -1 : 0;
}
