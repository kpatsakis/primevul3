static void nfc_llcp_recv_snl(struct nfc_llcp_local *local,
			      struct sk_buff *skb)
{
	struct nfc_llcp_sock *llcp_sock;
	u8 dsap, ssap, *tlv, type, length, tid, sap;
	u16 tlv_len, offset;
	char *service_name;
	size_t service_name_len;
	struct nfc_llcp_sdp_tlv *sdp;
	HLIST_HEAD(llc_sdres_list);
	size_t sdres_tlvs_len;
	HLIST_HEAD(nl_sdres_list);

	dsap = nfc_llcp_dsap(skb);
	ssap = nfc_llcp_ssap(skb);

	pr_debug("%d %d\n", dsap, ssap);

	if (dsap != LLCP_SAP_SDP || ssap != LLCP_SAP_SDP) {
		pr_err("Wrong SNL SAP\n");
		return;
	}

	tlv = &skb->data[LLCP_HEADER_SIZE];
	tlv_len = skb->len - LLCP_HEADER_SIZE;
	offset = 0;
	sdres_tlvs_len = 0;

	while (offset < tlv_len) {
		type = tlv[0];
		length = tlv[1];

		switch (type) {
		case LLCP_TLV_SDREQ:
			tid = tlv[2];
			service_name = (char *) &tlv[3];
			service_name_len = length - 1;

			pr_debug("Looking for %.16s\n", service_name);

			if (service_name_len == strlen("urn:nfc:sn:sdp") &&
			    !strncmp(service_name, "urn:nfc:sn:sdp",
				     service_name_len)) {
				sap = 1;
				goto add_snl;
			}

			llcp_sock = nfc_llcp_sock_from_sn(local, service_name,
							  service_name_len);
			if (!llcp_sock) {
				sap = 0;
				goto add_snl;
			}

			/*
			 * We found a socket but its ssap has not been reserved
			 * yet. We need to assign it for good and send a reply.
			 * The ssap will be freed when the socket is closed.
			 */
			if (llcp_sock->ssap == LLCP_SDP_UNBOUND) {
				atomic_t *client_count;

				sap = nfc_llcp_reserve_sdp_ssap(local);

				pr_debug("Reserving %d\n", sap);

				if (sap == LLCP_SAP_MAX) {
					sap = 0;
					goto add_snl;
				}

				client_count =
					&local->local_sdp_cnt[sap -
							      LLCP_WKS_NUM_SAP];

				atomic_inc(client_count);

				llcp_sock->ssap = sap;
				llcp_sock->reserved_ssap = sap;
			} else {
				sap = llcp_sock->ssap;
			}

			pr_debug("%p %d\n", llcp_sock, sap);

add_snl:
			sdp = nfc_llcp_build_sdres_tlv(tid, sap);
			if (sdp == NULL)
				goto exit;

			sdres_tlvs_len += sdp->tlv_len;
			hlist_add_head(&sdp->node, &llc_sdres_list);
			break;

		case LLCP_TLV_SDRES:
			mutex_lock(&local->sdreq_lock);

			pr_debug("LLCP_TLV_SDRES: searching tid %d\n", tlv[2]);

			hlist_for_each_entry(sdp, &local->pending_sdreqs, node) {
				if (sdp->tid != tlv[2])
					continue;

				sdp->sap = tlv[3];

				pr_debug("Found: uri=%s, sap=%d\n",
					 sdp->uri, sdp->sap);

				hlist_del(&sdp->node);

				hlist_add_head(&sdp->node, &nl_sdres_list);

				break;
			}

			mutex_unlock(&local->sdreq_lock);
			break;

		default:
			pr_err("Invalid SNL tlv value 0x%x\n", type);
			break;
		}

		offset += length + 2;
		tlv += length + 2;
	}

exit:
	if (!hlist_empty(&nl_sdres_list))
		nfc_genl_llc_send_sdres(local->dev, &nl_sdres_list);

	if (!hlist_empty(&llc_sdres_list))
		nfc_llcp_send_snl_sdres(local, &llc_sdres_list, sdres_tlvs_len);
}
