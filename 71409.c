static ssize_t ib_uverbs_write(struct file *filp, const char __user *buf,
			     size_t count, loff_t *pos)
{
	struct ib_uverbs_file *file = filp->private_data;
	const struct uverbs_api_write_method *method_elm;
	struct uverbs_api *uapi = file->device->uapi;
	struct ib_uverbs_ex_cmd_hdr ex_hdr;
	struct ib_uverbs_cmd_hdr hdr;
	struct uverbs_attr_bundle bundle;
	int srcu_key;
	ssize_t ret;

	if (!ib_safe_file_access(filp)) {
		pr_err_once("uverbs_write: process %d (%s) changed security contexts after opening file descriptor, this is not allowed.\n",
			    task_tgid_vnr(current), current->comm);
		return -EACCES;
	}

	if (count < sizeof(hdr))
		return -EINVAL;

	if (copy_from_user(&hdr, buf, sizeof(hdr)))
		return -EFAULT;

	method_elm = uapi_get_method(uapi, hdr.command);
	if (IS_ERR(method_elm))
		return PTR_ERR(method_elm);

	if (method_elm->is_ex) {
		if (count < (sizeof(hdr) + sizeof(ex_hdr)))
			return -EINVAL;
		if (copy_from_user(&ex_hdr, buf + sizeof(hdr), sizeof(ex_hdr)))
			return -EFAULT;
	}

	ret = verify_hdr(&hdr, &ex_hdr, count, method_elm);
	if (ret)
		return ret;

	srcu_key = srcu_read_lock(&file->device->disassociate_srcu);

	buf += sizeof(hdr);

	memset(bundle.attr_present, 0, sizeof(bundle.attr_present));
	bundle.ufile = file;
	bundle.context = NULL; /* only valid if bundle has uobject */
	if (!method_elm->is_ex) {
		size_t in_len = hdr.in_words * 4 - sizeof(hdr);
		size_t out_len = hdr.out_words * 4;
		u64 response = 0;

		if (method_elm->has_udata) {
			bundle.driver_udata.inlen =
				in_len - method_elm->req_size;
			in_len = method_elm->req_size;
			if (bundle.driver_udata.inlen)
				bundle.driver_udata.inbuf = buf + in_len;
			else
				bundle.driver_udata.inbuf = NULL;
		} else {
			memset(&bundle.driver_udata, 0,
			       sizeof(bundle.driver_udata));
		}

		if (method_elm->has_resp) {
			/*
			 * The macros check that if has_resp is set
			 * then the command request structure starts
			 * with a '__aligned u64 response' member.
			 */
			ret = get_user(response, (const u64 *)buf);
			if (ret)
				goto out_unlock;

			if (method_elm->has_udata) {
				bundle.driver_udata.outlen =
					out_len - method_elm->resp_size;
				out_len = method_elm->resp_size;
				if (bundle.driver_udata.outlen)
					bundle.driver_udata.outbuf =
						u64_to_user_ptr(response +
								out_len);
				else
					bundle.driver_udata.outbuf = NULL;
			}
		} else {
			bundle.driver_udata.outlen = 0;
			bundle.driver_udata.outbuf = NULL;
		}

		ib_uverbs_init_udata_buf_or_null(
			&bundle.ucore, buf, u64_to_user_ptr(response),
			in_len, out_len);
	} else {
		buf += sizeof(ex_hdr);

		ib_uverbs_init_udata_buf_or_null(&bundle.ucore, buf,
					u64_to_user_ptr(ex_hdr.response),
					hdr.in_words * 8, hdr.out_words * 8);

		ib_uverbs_init_udata_buf_or_null(
			&bundle.driver_udata, buf + bundle.ucore.inlen,
			u64_to_user_ptr(ex_hdr.response) + bundle.ucore.outlen,
			ex_hdr.provider_in_words * 8,
			ex_hdr.provider_out_words * 8);

	}

	ret = method_elm->handler(&bundle);
out_unlock:
	srcu_read_unlock(&file->device->disassociate_srcu, srcu_key);
	return (ret) ? : count;
}
