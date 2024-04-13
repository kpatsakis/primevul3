__smb2_find_mid(struct TCP_Server_Info *server, char *buf, bool dequeue)
{
	struct mid_q_entry *mid;
	struct smb2_hdr *shdr = (struct smb2_hdr *)buf;
	__u64 wire_mid = le64_to_cpu(shdr->MessageId);

	if (shdr->ProtocolId == SMB2_TRANSFORM_PROTO_NUM) {
		cifs_server_dbg(VFS, "Encrypted frame parsing not supported yet\n");
		return NULL;
	}

	spin_lock(&GlobalMid_Lock);
	list_for_each_entry(mid, &server->pending_mid_q, qhead) {
		if ((mid->mid == wire_mid) &&
		    (mid->mid_state == MID_REQUEST_SUBMITTED) &&
		    (mid->command == shdr->Command)) {
			kref_get(&mid->refcount);
			if (dequeue) {
				list_del_init(&mid->qhead);
				mid->mid_flags |= MID_DELETED;
			}
			spin_unlock(&GlobalMid_Lock);
			return mid;
		}
	}
	spin_unlock(&GlobalMid_Lock);
	return NULL;
}