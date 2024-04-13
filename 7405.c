parse_server_interfaces(struct network_interface_info_ioctl_rsp *buf,
			size_t buf_len,
			struct cifs_server_iface **iface_list,
			size_t *iface_count)
{
	struct network_interface_info_ioctl_rsp *p;
	struct sockaddr_in *addr4;
	struct sockaddr_in6 *addr6;
	struct iface_info_ipv4 *p4;
	struct iface_info_ipv6 *p6;
	struct cifs_server_iface *info;
	ssize_t bytes_left;
	size_t next = 0;
	int nb_iface = 0;
	int rc = 0;

	*iface_list = NULL;
	*iface_count = 0;

	/*
	 * Fist pass: count and sanity check
	 */

	bytes_left = buf_len;
	p = buf;
	while (bytes_left >= sizeof(*p)) {
		nb_iface++;
		next = le32_to_cpu(p->Next);
		if (!next) {
			bytes_left -= sizeof(*p);
			break;
		}
		p = (struct network_interface_info_ioctl_rsp *)((u8 *)p+next);
		bytes_left -= next;
	}

	if (!nb_iface) {
		cifs_dbg(VFS, "%s: malformed interface info\n", __func__);
		rc = -EINVAL;
		goto out;
	}

	/* Azure rounds the buffer size up 8, to a 16 byte boundary */
	if ((bytes_left > 8) || p->Next)
		cifs_dbg(VFS, "%s: incomplete interface info\n", __func__);


	/*
	 * Second pass: extract info to internal structure
	 */

	*iface_list = kcalloc(nb_iface, sizeof(**iface_list), GFP_KERNEL);
	if (!*iface_list) {
		rc = -ENOMEM;
		goto out;
	}

	info = *iface_list;
	bytes_left = buf_len;
	p = buf;
	while (bytes_left >= sizeof(*p)) {
		info->speed = le64_to_cpu(p->LinkSpeed);
		info->rdma_capable = le32_to_cpu(p->Capability & RDMA_CAPABLE) ? 1 : 0;
		info->rss_capable = le32_to_cpu(p->Capability & RSS_CAPABLE) ? 1 : 0;

		cifs_dbg(FYI, "%s: adding iface %zu\n", __func__, *iface_count);
		cifs_dbg(FYI, "%s: speed %zu bps\n", __func__, info->speed);
		cifs_dbg(FYI, "%s: capabilities 0x%08x\n", __func__,
			 le32_to_cpu(p->Capability));

		switch (p->Family) {
		/*
		 * The kernel and wire socket structures have the same
		 * layout and use network byte order but make the
		 * conversion explicit in case either one changes.
		 */
		case INTERNETWORK:
			addr4 = (struct sockaddr_in *)&info->sockaddr;
			p4 = (struct iface_info_ipv4 *)p->Buffer;
			addr4->sin_family = AF_INET;
			memcpy(&addr4->sin_addr, &p4->IPv4Address, 4);

			/* [MS-SMB2] 2.2.32.5.1.1 Clients MUST ignore these */
			addr4->sin_port = cpu_to_be16(CIFS_PORT);

			cifs_dbg(FYI, "%s: ipv4 %pI4\n", __func__,
				 &addr4->sin_addr);
			break;
		case INTERNETWORKV6:
			addr6 =	(struct sockaddr_in6 *)&info->sockaddr;
			p6 = (struct iface_info_ipv6 *)p->Buffer;
			addr6->sin6_family = AF_INET6;
			memcpy(&addr6->sin6_addr, &p6->IPv6Address, 16);

			/* [MS-SMB2] 2.2.32.5.1.2 Clients MUST ignore these */
			addr6->sin6_flowinfo = 0;
			addr6->sin6_scope_id = 0;
			addr6->sin6_port = cpu_to_be16(CIFS_PORT);

			cifs_dbg(FYI, "%s: ipv6 %pI6\n", __func__,
				 &addr6->sin6_addr);
			break;
		default:
			cifs_dbg(VFS,
				 "%s: skipping unsupported socket family\n",
				 __func__);
			goto next_iface;
		}

		(*iface_count)++;
		info++;
next_iface:
		next = le32_to_cpu(p->Next);
		if (!next)
			break;
		p = (struct network_interface_info_ioctl_rsp *)((u8 *)p+next);
		bytes_left -= next;
	}

	if (!*iface_count) {
		rc = -EINVAL;
		goto out;
	}

out:
	if (rc) {
		kfree(*iface_list);
		*iface_count = 0;
		*iface_list = NULL;
	}
	return rc;
}