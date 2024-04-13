static int add_ref_from_push_spec(git_vector *refs, push_spec *push_spec)
{
	git_pkt_ref *added = git__calloc(1, sizeof(git_pkt_ref));
	GITERR_CHECK_ALLOC(added);

	added->type = GIT_PKT_REF;
	git_oid_cpy(&added->head.oid, &push_spec->loid);
	added->head.name = git__strdup(push_spec->refspec.dst);

	if (!added->head.name ||
		git_vector_insert(refs, added) < 0) {
		git_pkt_free((git_pkt *)added);
		return -1;
	}

	return 0;
}
