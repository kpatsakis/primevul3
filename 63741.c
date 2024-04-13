static int fsck_tag_buffer(struct tag *tag, const char *data,
	unsigned long size, struct fsck_options *options)
{
	unsigned char sha1[20];
	int ret = 0;
	const char *buffer;
	char *to_free = NULL, *eol;
	struct strbuf sb = STRBUF_INIT;

	if (data)
		buffer = data;
	else {
		enum object_type type;

		buffer = to_free =
			read_sha1_file(tag->object.oid.hash, &type, &size);
		if (!buffer)
			return report(options, &tag->object,
				FSCK_MSG_MISSING_TAG_OBJECT,
				"cannot read tag object");

		if (type != OBJ_TAG) {
			ret = report(options, &tag->object,
				FSCK_MSG_TAG_OBJECT_NOT_TAG,
				"expected tag got %s",
			    type_name(type));
			goto done;
		}
	}

	ret = verify_headers(buffer, size, &tag->object, options);
	if (ret)
		goto done;

	if (!skip_prefix(buffer, "object ", &buffer)) {
		ret = report(options, &tag->object, FSCK_MSG_MISSING_OBJECT, "invalid format - expected 'object' line");
		goto done;
	}
	if (get_sha1_hex(buffer, sha1) || buffer[40] != '\n') {
		ret = report(options, &tag->object, FSCK_MSG_BAD_OBJECT_SHA1, "invalid 'object' line format - bad sha1");
		if (ret)
			goto done;
	}
	buffer += 41;

	if (!skip_prefix(buffer, "type ", &buffer)) {
		ret = report(options, &tag->object, FSCK_MSG_MISSING_TYPE_ENTRY, "invalid format - expected 'type' line");
		goto done;
	}
	eol = strchr(buffer, '\n');
	if (!eol) {
		ret = report(options, &tag->object, FSCK_MSG_MISSING_TYPE, "invalid format - unexpected end after 'type' line");
		goto done;
	}
	if (type_from_string_gently(buffer, eol - buffer, 1) < 0)
		ret = report(options, &tag->object, FSCK_MSG_BAD_TYPE, "invalid 'type' value");
	if (ret)
		goto done;
	buffer = eol + 1;

	if (!skip_prefix(buffer, "tag ", &buffer)) {
		ret = report(options, &tag->object, FSCK_MSG_MISSING_TAG_ENTRY, "invalid format - expected 'tag' line");
		goto done;
	}
	eol = strchr(buffer, '\n');
	if (!eol) {
		ret = report(options, &tag->object, FSCK_MSG_MISSING_TAG, "invalid format - unexpected end after 'type' line");
		goto done;
	}
	strbuf_addf(&sb, "refs/tags/%.*s", (int)(eol - buffer), buffer);
	if (check_refname_format(sb.buf, 0)) {
		ret = report(options, &tag->object, FSCK_MSG_BAD_TAG_NAME,
			   "invalid 'tag' name: %.*s",
			   (int)(eol - buffer), buffer);
		if (ret)
			goto done;
	}
	buffer = eol + 1;

	if (!skip_prefix(buffer, "tagger ", &buffer)) {
		/* early tags do not contain 'tagger' lines; warn only */
		ret = report(options, &tag->object, FSCK_MSG_MISSING_TAGGER_ENTRY, "invalid format - expected 'tagger' line");
		if (ret)
			goto done;
	}
	else
		ret = fsck_ident(&buffer, &tag->object, options);

done:
	strbuf_release(&sb);
	free(to_free);
	return ret;
}
