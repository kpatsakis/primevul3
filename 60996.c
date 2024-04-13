int git_smart__detect_caps(git_pkt_ref *pkt, transport_smart_caps *caps, git_vector *symrefs)
{
	const char *ptr;

	/* No refs or capabilites, odd but not a problem */
	if (pkt == NULL || pkt->capabilities == NULL)
		return 0;

	ptr = pkt->capabilities;
	while (ptr != NULL && *ptr != '\0') {
		if (*ptr == ' ')
			ptr++;

		if (!git__prefixcmp(ptr, GIT_CAP_OFS_DELTA)) {
			caps->common = caps->ofs_delta = 1;
			ptr += strlen(GIT_CAP_OFS_DELTA);
			continue;
		}

		/* Keep multi_ack_detailed before multi_ack */
		if (!git__prefixcmp(ptr, GIT_CAP_MULTI_ACK_DETAILED)) {
			caps->common = caps->multi_ack_detailed = 1;
			ptr += strlen(GIT_CAP_MULTI_ACK_DETAILED);
			continue;
		}

		if (!git__prefixcmp(ptr, GIT_CAP_MULTI_ACK)) {
			caps->common = caps->multi_ack = 1;
			ptr += strlen(GIT_CAP_MULTI_ACK);
			continue;
		}

		if (!git__prefixcmp(ptr, GIT_CAP_INCLUDE_TAG)) {
			caps->common = caps->include_tag = 1;
			ptr += strlen(GIT_CAP_INCLUDE_TAG);
			continue;
		}

		/* Keep side-band check after side-band-64k */
		if (!git__prefixcmp(ptr, GIT_CAP_SIDE_BAND_64K)) {
			caps->common = caps->side_band_64k = 1;
			ptr += strlen(GIT_CAP_SIDE_BAND_64K);
			continue;
		}

		if (!git__prefixcmp(ptr, GIT_CAP_SIDE_BAND)) {
			caps->common = caps->side_band = 1;
			ptr += strlen(GIT_CAP_SIDE_BAND);
			continue;
		}

		if (!git__prefixcmp(ptr, GIT_CAP_DELETE_REFS)) {
			caps->common = caps->delete_refs = 1;
			ptr += strlen(GIT_CAP_DELETE_REFS);
			continue;
		}

		if (!git__prefixcmp(ptr, GIT_CAP_THIN_PACK)) {
			caps->common = caps->thin_pack = 1;
			ptr += strlen(GIT_CAP_THIN_PACK);
			continue;
		}

		if (!git__prefixcmp(ptr, GIT_CAP_SYMREF)) {
			int error;

			if ((error = append_symref(&ptr, symrefs, ptr)) < 0)
				return error;

			continue;
		}

		/* We don't know this capability, so skip it */
		ptr = strchr(ptr, ' ');
	}

	return 0;
}
