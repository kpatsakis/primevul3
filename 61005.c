static int wait_while_ack(gitno_buffer *buf)
{
	int error;
	git_pkt_ack *pkt = NULL;

	while (1) {
		git__free(pkt);

		if ((error = recv_pkt((git_pkt **)&pkt, buf)) < 0)
			return error;

		if (pkt->type == GIT_PKT_NAK)
			break;

		if (pkt->type == GIT_PKT_ACK &&
		    (pkt->status != GIT_ACK_CONTINUE &&
		     pkt->status != GIT_ACK_COMMON)) {
			git__free(pkt);
			return 0;
		}
	}

	git__free(pkt);
	return 0;
}
