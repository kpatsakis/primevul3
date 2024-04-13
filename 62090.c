list_item_verbose(struct bsdtar *bsdtar, FILE *out, struct archive_entry *entry)
{
	char			 tmp[100];
	size_t			 w;
	const char		*p;
	const char		*fmt;
	time_t			 tim;
	static time_t		 now;

	/*
	 * We avoid collecting the entire list in memory at once by
	 * listing things as we see them.  However, that also means we can't
	 * just pre-compute the field widths.  Instead, we start with guesses
	 * and just widen them as necessary.  These numbers are completely
	 * arbitrary.
	 */
	if (!bsdtar->u_width) {
		bsdtar->u_width = 6;
		bsdtar->gs_width = 13;
	}
	if (!now)
		time(&now);
	fprintf(out, "%s %d ",
	    archive_entry_strmode(entry),
	    archive_entry_nlink(entry));

	/* Use uname if it's present, else uid. */
	p = archive_entry_uname(entry);
	if ((p == NULL) || (*p == '\0')) {
		sprintf(tmp, "%lu ",
		    (unsigned long)archive_entry_uid(entry));
		p = tmp;
	}
	w = strlen(p);
	if (w > bsdtar->u_width)
		bsdtar->u_width = w;
	fprintf(out, "%-*s ", (int)bsdtar->u_width, p);

	/* Use gname if it's present, else gid. */
	p = archive_entry_gname(entry);
	if (p != NULL && p[0] != '\0') {
		fprintf(out, "%s", p);
		w = strlen(p);
	} else {
		sprintf(tmp, "%lu",
		    (unsigned long)archive_entry_gid(entry));
		w = strlen(tmp);
		fprintf(out, "%s", tmp);
	}

	/*
	 * Print device number or file size, right-aligned so as to make
	 * total width of group and devnum/filesize fields be gs_width.
	 * If gs_width is too small, grow it.
	 */
	if (archive_entry_filetype(entry) == AE_IFCHR
	    || archive_entry_filetype(entry) == AE_IFBLK) {
		sprintf(tmp, "%lu,%lu",
		    (unsigned long)archive_entry_rdevmajor(entry),
		    (unsigned long)archive_entry_rdevminor(entry));
	} else {
		strcpy(tmp, tar_i64toa(archive_entry_size(entry)));
	}
	if (w + strlen(tmp) >= bsdtar->gs_width)
		bsdtar->gs_width = w+strlen(tmp)+1;
	fprintf(out, "%*s", (int)(bsdtar->gs_width - w), tmp);

	/* Format the time using 'ls -l' conventions. */
	tim = archive_entry_mtime(entry);
#define	HALF_YEAR (time_t)365 * 86400 / 2
#if defined(_WIN32) && !defined(__CYGWIN__)
#define	DAY_FMT  "%d"  /* Windows' strftime function does not support %e format. */
#else
#define	DAY_FMT  "%e"  /* Day number without leading zeros */
#endif
	if (tim < now - HALF_YEAR || tim > now + HALF_YEAR)
		fmt = bsdtar->day_first ? DAY_FMT " %b  %Y" : "%b " DAY_FMT "  %Y";
	else
		fmt = bsdtar->day_first ? DAY_FMT " %b %H:%M" : "%b " DAY_FMT " %H:%M";
	strftime(tmp, sizeof(tmp), fmt, localtime(&tim));
	fprintf(out, " %s ", tmp);
	safe_fprintf(out, "%s", archive_entry_pathname(entry));

	/* Extra information for links. */
	if (archive_entry_hardlink(entry)) /* Hard link */
		safe_fprintf(out, " link to %s",
		    archive_entry_hardlink(entry));
	else if (archive_entry_symlink(entry)) /* Symbolic link */
		safe_fprintf(out, " -> %s", archive_entry_symlink(entry));
}
