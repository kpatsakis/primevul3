move_smb2_ea_to_cifs(char *dst, size_t dst_size,
		     struct smb2_file_full_ea_info *src, size_t src_size,
		     const unsigned char *ea_name)
{
	int rc = 0;
	unsigned int ea_name_len = ea_name ? strlen(ea_name) : 0;
	char *name, *value;
	size_t buf_size = dst_size;
	size_t name_len, value_len, user_name_len;

	while (src_size > 0) {
		name = &src->ea_data[0];
		name_len = (size_t)src->ea_name_length;
		value = &src->ea_data[src->ea_name_length + 1];
		value_len = (size_t)le16_to_cpu(src->ea_value_length);

		if (name_len == 0)
			break;

		if (src_size < 8 + name_len + 1 + value_len) {
			cifs_dbg(FYI, "EA entry goes beyond length of list\n");
			rc = -EIO;
			goto out;
		}

		if (ea_name) {
			if (ea_name_len == name_len &&
			    memcmp(ea_name, name, name_len) == 0) {
				rc = value_len;
				if (dst_size == 0)
					goto out;
				if (dst_size < value_len) {
					rc = -ERANGE;
					goto out;
				}
				memcpy(dst, value, value_len);
				goto out;
			}
		} else {
			/* 'user.' plus a terminating null */
			user_name_len = 5 + 1 + name_len;

			if (buf_size == 0) {
				/* skip copy - calc size only */
				rc += user_name_len;
			} else if (dst_size >= user_name_len) {
				dst_size -= user_name_len;
				memcpy(dst, "user.", 5);
				dst += 5;
				memcpy(dst, src->ea_data, name_len);
				dst += name_len;
				*dst = 0;
				++dst;
				rc += user_name_len;
			} else {
				/* stop before overrun buffer */
				rc = -ERANGE;
				break;
			}
		}

		if (!src->next_entry_offset)
			break;

		if (src_size < le32_to_cpu(src->next_entry_offset)) {
			/* stop before overrun buffer */
			rc = -ERANGE;
			break;
		}
		src_size -= le32_to_cpu(src->next_entry_offset);
		src = (void *)((char *)src +
			       le32_to_cpu(src->next_entry_offset));
	}

	/* didn't find the named attribute */
	if (ea_name)
		rc = -ENODATA;

out:
	return (ssize_t)rc;
}