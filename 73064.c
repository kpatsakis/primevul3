static ssize_t tun_get_user(struct tun_struct *tun, struct tun_file *tfile,
			    void *msg_control, struct iov_iter *from,
			    int noblock, bool more)
{
	struct tun_pi pi = { 0, cpu_to_be16(ETH_P_IP) };
	struct sk_buff *skb;
	size_t total_len = iov_iter_count(from);
	size_t len = total_len, align = tun->align, linear;
	struct virtio_net_hdr gso = { 0 };
	struct tun_pcpu_stats *stats;
	int good_linear;
	int copylen;
	bool zerocopy = false;
	int err;
	u32 rxhash;
	int skb_xdp = 1;

	if (!(tun->dev->flags & IFF_UP))
		return -EIO;

	if (!(tun->flags & IFF_NO_PI)) {
		if (len < sizeof(pi))
			return -EINVAL;
		len -= sizeof(pi);

		if (!copy_from_iter_full(&pi, sizeof(pi), from))
			return -EFAULT;
	}

	if (tun->flags & IFF_VNET_HDR) {
		int vnet_hdr_sz = READ_ONCE(tun->vnet_hdr_sz);

		if (len < vnet_hdr_sz)
			return -EINVAL;
		len -= vnet_hdr_sz;

		if (!copy_from_iter_full(&gso, sizeof(gso), from))
			return -EFAULT;

		if ((gso.flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) &&
		    tun16_to_cpu(tun, gso.csum_start) + tun16_to_cpu(tun, gso.csum_offset) + 2 > tun16_to_cpu(tun, gso.hdr_len))
			gso.hdr_len = cpu_to_tun16(tun, tun16_to_cpu(tun, gso.csum_start) + tun16_to_cpu(tun, gso.csum_offset) + 2);

		if (tun16_to_cpu(tun, gso.hdr_len) > len)
			return -EINVAL;
		iov_iter_advance(from, vnet_hdr_sz - sizeof(gso));
	}

	if ((tun->flags & TUN_TYPE_MASK) == IFF_TAP) {
		align += NET_IP_ALIGN;
		if (unlikely(len < ETH_HLEN ||
			     (gso.hdr_len && tun16_to_cpu(tun, gso.hdr_len) < ETH_HLEN)))
			return -EINVAL;
	}

	good_linear = SKB_MAX_HEAD(align);

	if (msg_control) {
		struct iov_iter i = *from;

		/* There are 256 bytes to be copied in skb, so there is
		 * enough room for skb expand head in case it is used.
		 * The rest of the buffer is mapped from userspace.
		 */
		copylen = gso.hdr_len ? tun16_to_cpu(tun, gso.hdr_len) : GOODCOPY_LEN;
		if (copylen > good_linear)
			copylen = good_linear;
		linear = copylen;
		iov_iter_advance(&i, copylen);
		if (iov_iter_npages(&i, INT_MAX) <= MAX_SKB_FRAGS)
			zerocopy = true;
	}

	if (tun_can_build_skb(tun, tfile, len, noblock, zerocopy)) {
		/* For the packet that is not easy to be processed
		 * (e.g gso or jumbo packet), we will do it at after
		 * skb was created with generic XDP routine.
		 */
		skb = tun_build_skb(tun, tfile, from, &gso, len, &skb_xdp);
		if (IS_ERR(skb)) {
			this_cpu_inc(tun->pcpu_stats->rx_dropped);
			return PTR_ERR(skb);
		}
		if (!skb)
			return total_len;
	} else {
		if (!zerocopy) {
			copylen = len;
			if (tun16_to_cpu(tun, gso.hdr_len) > good_linear)
				linear = good_linear;
			else
				linear = tun16_to_cpu(tun, gso.hdr_len);
		}

		skb = tun_alloc_skb(tfile, align, copylen, linear, noblock);
		if (IS_ERR(skb)) {
			if (PTR_ERR(skb) != -EAGAIN)
				this_cpu_inc(tun->pcpu_stats->rx_dropped);
			return PTR_ERR(skb);
		}

		if (zerocopy)
			err = zerocopy_sg_from_iter(skb, from);
		else
			err = skb_copy_datagram_from_iter(skb, 0, from, len);

		if (err) {
			this_cpu_inc(tun->pcpu_stats->rx_dropped);
			kfree_skb(skb);
			return -EFAULT;
		}
	}

	if (virtio_net_hdr_to_skb(skb, &gso, tun_is_little_endian(tun))) {
		this_cpu_inc(tun->pcpu_stats->rx_frame_errors);
		kfree_skb(skb);
		return -EINVAL;
	}

	switch (tun->flags & TUN_TYPE_MASK) {
	case IFF_TUN:
		if (tun->flags & IFF_NO_PI) {
			u8 ip_version = skb->len ? (skb->data[0] >> 4) : 0;

			switch (ip_version) {
			case 4:
				pi.proto = htons(ETH_P_IP);
				break;
			case 6:
				pi.proto = htons(ETH_P_IPV6);
				break;
			default:
				this_cpu_inc(tun->pcpu_stats->rx_dropped);
				kfree_skb(skb);
				return -EINVAL;
			}
		}

		skb_reset_mac_header(skb);
		skb->protocol = pi.proto;
		skb->dev = tun->dev;
		break;
	case IFF_TAP:
		skb->protocol = eth_type_trans(skb, tun->dev);
		break;
	}

	/* copy skb_ubuf_info for callback when skb has no error */
	if (zerocopy) {
		skb_shinfo(skb)->destructor_arg = msg_control;
		skb_shinfo(skb)->tx_flags |= SKBTX_DEV_ZEROCOPY;
		skb_shinfo(skb)->tx_flags |= SKBTX_SHARED_FRAG;
	} else if (msg_control) {
		struct ubuf_info *uarg = msg_control;
		uarg->callback(uarg, false);
	}

	skb_reset_network_header(skb);
	skb_probe_transport_header(skb, 0);

	if (skb_xdp) {
		struct bpf_prog *xdp_prog;
		int ret;

		rcu_read_lock();
		xdp_prog = rcu_dereference(tun->xdp_prog);
		if (xdp_prog) {
			ret = do_xdp_generic(xdp_prog, skb);
			if (ret != XDP_PASS) {
				rcu_read_unlock();
				return total_len;
			}
		}
		rcu_read_unlock();
	}

	rxhash = __skb_get_hash_symmetric(skb);
#ifndef CONFIG_4KSTACKS
	tun_rx_batched(tun, tfile, skb, more);
#else
	netif_rx_ni(skb);
#endif

	stats = get_cpu_ptr(tun->pcpu_stats);
	u64_stats_update_begin(&stats->syncp);
	stats->rx_packets++;
	stats->rx_bytes += len;
	u64_stats_update_end(&stats->syncp);
	put_cpu_ptr(stats);

	tun_flow_update(tun, rxhash, tfile);
	return total_len;
}
