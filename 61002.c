static int store_common(transport_smart *t)
{
	git_pkt *pkt = NULL;
	gitno_buffer *buf = &t->buffer;
	int error;

	do {
		if ((error = recv_pkt(&pkt, buf)) < 0)
			return error;

		if (pkt->type == GIT_PKT_ACK) {
			if (git_vector_insert(&t->common, pkt) < 0)
				return -1;
		} else {
			git__free(pkt);
			return 0;
		}

	} while (1);

	return 0;
}
