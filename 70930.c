static void nfc_llcp_recv_connect(struct nfc_llcp_local *local,
				  struct sk_buff *skb)
{
	struct sock *new_sk, *parent;
	struct nfc_llcp_sock *sock, *new_sock;
	u8 dsap, ssap, reason;

	dsap = nfc_llcp_dsap(skb);
	ssap = nfc_llcp_ssap(skb);

	pr_debug("%d %d\n", dsap, ssap);

	if (dsap != LLCP_SAP_SDP) {
		sock = nfc_llcp_sock_get(local, dsap, LLCP_SAP_SDP);
		if (sock == NULL || sock->sk.sk_state != LLCP_LISTEN) {
			reason = LLCP_DM_NOBOUND;
			goto fail;
		}
	} else {
		u8 *sn;
		size_t sn_len;

		sn = nfc_llcp_connect_sn(skb, &sn_len);
		if (sn == NULL) {
			reason = LLCP_DM_NOBOUND;
			goto fail;
		}

		pr_debug("Service name length %zu\n", sn_len);

		sock = nfc_llcp_sock_get_sn(local, sn, sn_len);
		if (sock == NULL) {
			reason = LLCP_DM_NOBOUND;
			goto fail;
		}
	}

	lock_sock(&sock->sk);

	parent = &sock->sk;

	if (sk_acceptq_is_full(parent)) {
		reason = LLCP_DM_REJ;
		release_sock(&sock->sk);
		sock_put(&sock->sk);
		goto fail;
	}

	if (sock->ssap == LLCP_SDP_UNBOUND) {
		u8 ssap = nfc_llcp_reserve_sdp_ssap(local);

		pr_debug("First client, reserving %d\n", ssap);

		if (ssap == LLCP_SAP_MAX) {
			reason = LLCP_DM_REJ;
			release_sock(&sock->sk);
			sock_put(&sock->sk);
			goto fail;
		}

		sock->ssap = ssap;
	}

	new_sk = nfc_llcp_sock_alloc(NULL, parent->sk_type, GFP_ATOMIC, 0);
	if (new_sk == NULL) {
		reason = LLCP_DM_REJ;
		release_sock(&sock->sk);
		sock_put(&sock->sk);
		goto fail;
	}

	new_sock = nfc_llcp_sock(new_sk);
	new_sock->dev = local->dev;
	new_sock->local = nfc_llcp_local_get(local);
	new_sock->rw = sock->rw;
	new_sock->miux = sock->miux;
	new_sock->nfc_protocol = sock->nfc_protocol;
	new_sock->dsap = ssap;
	new_sock->target_idx = local->target_idx;
	new_sock->parent = parent;
	new_sock->ssap = sock->ssap;
	if (sock->ssap < LLCP_LOCAL_NUM_SAP && sock->ssap >= LLCP_WKS_NUM_SAP) {
		atomic_t *client_count;

		pr_debug("reserved_ssap %d for %p\n", sock->ssap, new_sock);

		client_count =
			&local->local_sdp_cnt[sock->ssap - LLCP_WKS_NUM_SAP];

		atomic_inc(client_count);
		new_sock->reserved_ssap = sock->ssap;
	}

	nfc_llcp_parse_connection_tlv(new_sock, &skb->data[LLCP_HEADER_SIZE],
				      skb->len - LLCP_HEADER_SIZE);

	pr_debug("new sock %p sk %p\n", new_sock, &new_sock->sk);

	nfc_llcp_sock_link(&local->sockets, new_sk);

	nfc_llcp_accept_enqueue(&sock->sk, new_sk);

	nfc_get_device(local->dev->idx);

	new_sk->sk_state = LLCP_CONNECTED;

	/* Wake the listening processes */
	parent->sk_data_ready(parent);

	/* Send CC */
	nfc_llcp_send_cc(new_sock);

	release_sock(&sock->sk);
	sock_put(&sock->sk);

	return;

fail:
	/* Send DM */
	nfc_llcp_send_dm(local, dsap, ssap, reason);
}
