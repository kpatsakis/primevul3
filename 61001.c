static int no_sideband(transport_smart *t, struct git_odb_writepack *writepack, gitno_buffer *buf, git_transfer_progress *stats)
{
	int recvd;

	do {
		if (t->cancelled.val) {
			giterr_set(GITERR_NET, "The fetch was cancelled by the user");
			return GIT_EUSER;
		}

		if (writepack->append(writepack, buf->data, buf->offset, stats) < 0)
			return -1;

		gitno_consume_n(buf, buf->offset);

		if ((recvd = gitno_recv(buf)) < 0)
			return recvd;
	} while(recvd > 0);

	if (writepack->commit(writepack, stats) < 0)
		return -1;

	return 0;
}
