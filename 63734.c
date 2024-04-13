int fsck_finish(struct fsck_options *options)
{
	int ret = 0;
	struct oidset_iter iter;
	const struct object_id *oid;

	oidset_iter_init(&gitmodules_found, &iter);
	while ((oid = oidset_iter_next(&iter))) {
		struct blob *blob;
		enum object_type type;
		unsigned long size;
		char *buf;

		if (oidset_contains(&gitmodules_done, oid))
			continue;

		blob = lookup_blob(oid);
		if (!blob) {
			ret |= report(options, &blob->object,
				      FSCK_MSG_GITMODULES_BLOB,
				      "non-blob found at .gitmodules");
			continue;
		}

		buf = read_sha1_file(oid->hash, &type, &size);
		if (!buf) {
			if (is_promisor_object(&blob->object.oid))
				continue;
			ret |= report(options, &blob->object,
				      FSCK_MSG_GITMODULES_MISSING,
				      "unable to read .gitmodules blob");
			continue;
		}

		if (type == OBJ_BLOB)
			ret |= fsck_blob(blob, buf, size, options);
		else
			ret |= report(options, &blob->object,
				      FSCK_MSG_GITMODULES_BLOB,
				      "non-blob found at .gitmodules");
		free(buf);
	}


	oidset_clear(&gitmodules_found);
	oidset_clear(&gitmodules_done);
	return ret;
}
