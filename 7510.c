smb3_create_lease_buf(u8 *lease_key, u8 oplock)
{
	struct create_lease_v2 *buf;

	buf = kzalloc(sizeof(struct create_lease_v2), GFP_KERNEL);
	if (!buf)
		return NULL;

	memcpy(&buf->lcontext.LeaseKey, lease_key, SMB2_LEASE_KEY_SIZE);
	buf->lcontext.LeaseState = map_oplock_to_lease(oplock);

	buf->ccontext.DataOffset = cpu_to_le16(offsetof
					(struct create_lease_v2, lcontext));
	buf->ccontext.DataLength = cpu_to_le32(sizeof(struct lease_context_v2));
	buf->ccontext.NameOffset = cpu_to_le16(offsetof
				(struct create_lease_v2, Name));
	buf->ccontext.NameLength = cpu_to_le16(4);
	/* SMB2_CREATE_REQUEST_LEASE is "RqLs" */
	buf->Name[0] = 'R';
	buf->Name[1] = 'q';
	buf->Name[2] = 'L';
	buf->Name[3] = 's';
	return (char *)buf;
}