reply_parse(struct evdns_base *base, u8 *packet, int length) {
	int j = 0, k = 0;  /* index into packet */
	u16 t_;	 /* used by the macros */
	u32 t32_;  /* used by the macros */
	char tmp_name[256], cmp_name[256]; /* used by the macros */
	int name_matches = 0;

	u16 trans_id, questions, answers, authority, additional, datalength;
	u16 flags = 0;
	u32 ttl, ttl_r = 0xffffffff;
	struct reply reply;
	struct request *req = NULL;
	unsigned int i;

	ASSERT_LOCKED(base);

	GET16(trans_id);
	GET16(flags);
	GET16(questions);
	GET16(answers);
	GET16(authority);
	GET16(additional);
	(void) authority; /* suppress "unused variable" warnings. */
	(void) additional; /* suppress "unused variable" warnings. */

	req = request_find_from_trans_id(base, trans_id);
	if (!req) return -1;
	EVUTIL_ASSERT(req->base == base);

	memset(&reply, 0, sizeof(reply));

	/* If it's not an answer, it doesn't correspond to any request. */
	if (!(flags & 0x8000)) return -1;  /* must be an answer */
	if ((flags & 0x020f) && (flags & 0x020f) != DNS_ERR_NOTEXIST) {
		/* there was an error and it's not NXDOMAIN */
		goto err;
	}
	/* if (!answers) return; */  /* must have an answer of some form */

	/* This macro skips a name in the DNS reply. */
#define SKIP_NAME						\
	do { tmp_name[0] = '\0';				\
		if (name_parse(packet, length, &j, tmp_name,	\
			sizeof(tmp_name))<0)			\
			goto err;				\
	} while (0)

	reply.type = req->request_type;

	/* skip over each question in the reply */
	for (i = 0; i < questions; ++i) {
		/* the question looks like
		 *   <label:name><u16:type><u16:class>
		 */
		tmp_name[0] = '\0';
		cmp_name[0] = '\0';
		k = j;
		if (name_parse(packet, length, &j, tmp_name, sizeof(tmp_name)) < 0)
			goto err;
		if (name_parse(req->request, req->request_len, &k,
			cmp_name, sizeof(cmp_name))<0)
			goto err;
		if (!base->global_randomize_case) {
			if (strcmp(tmp_name, cmp_name) == 0)
				name_matches = 1;
		} else {
			if (evutil_ascii_strcasecmp(tmp_name, cmp_name) == 0)
				name_matches = 1;
		}

		j += 4;
		if (j > length)
			goto err;
	}

	if (!name_matches)
		goto err;

	/* now we have the answer section which looks like
	 * <label:name><u16:type><u16:class><u32:ttl><u16:len><data...>
	 */

	for (i = 0; i < answers; ++i) {
		u16 type, class;

		SKIP_NAME;
		GET16(type);
		GET16(class);
		GET32(ttl);
		GET16(datalength);

		if (type == TYPE_A && class == CLASS_INET) {
			int addrcount, addrtocopy;
			if (req->request_type != TYPE_A) {
				j += datalength; continue;
			}
			if ((datalength & 3) != 0) /* not an even number of As. */
			    goto err;
			addrcount = datalength >> 2;
			addrtocopy = MIN(MAX_V4_ADDRS - reply.data.a.addrcount, (unsigned)addrcount);

			ttl_r = MIN(ttl_r, ttl);
			/* we only bother with the first four addresses. */
			if (j + 4*addrtocopy > length) goto err;
			memcpy(&reply.data.a.addresses[reply.data.a.addrcount],
				   packet + j, 4*addrtocopy);
			j += 4*addrtocopy;
			reply.data.a.addrcount += addrtocopy;
			reply.have_answer = 1;
			if (reply.data.a.addrcount == MAX_V4_ADDRS) break;
		} else if (type == TYPE_PTR && class == CLASS_INET) {
			if (req->request_type != TYPE_PTR) {
				j += datalength; continue;
			}
			if (name_parse(packet, length, &j, reply.data.ptr.name,
						   sizeof(reply.data.ptr.name))<0)
				goto err;
			ttl_r = MIN(ttl_r, ttl);
			reply.have_answer = 1;
			break;
		} else if (type == TYPE_CNAME) {
			char cname[HOST_NAME_MAX];
			if (!req->put_cname_in_ptr || *req->put_cname_in_ptr) {
				j += datalength; continue;
			}
			if (name_parse(packet, length, &j, cname,
				sizeof(cname))<0)
				goto err;
			*req->put_cname_in_ptr = mm_strdup(cname);
		} else if (type == TYPE_AAAA && class == CLASS_INET) {
			int addrcount, addrtocopy;
			if (req->request_type != TYPE_AAAA) {
				j += datalength; continue;
			}
			if ((datalength & 15) != 0) /* not an even number of AAAAs. */
				goto err;
			addrcount = datalength >> 4;  /* each address is 16 bytes long */
			addrtocopy = MIN(MAX_V6_ADDRS - reply.data.aaaa.addrcount, (unsigned)addrcount);
			ttl_r = MIN(ttl_r, ttl);

			/* we only bother with the first four addresses. */
			if (j + 16*addrtocopy > length) goto err;
			memcpy(&reply.data.aaaa.addresses[reply.data.aaaa.addrcount],
				   packet + j, 16*addrtocopy);
			reply.data.aaaa.addrcount += addrtocopy;
			j += 16*addrtocopy;
			reply.have_answer = 1;
			if (reply.data.aaaa.addrcount == MAX_V6_ADDRS) break;
		} else {
			/* skip over any other type of resource */
			j += datalength;
		}
	}

	if (!reply.have_answer) {
		for (i = 0; i < authority; ++i) {
			u16 type, class;
			SKIP_NAME;
			GET16(type);
			GET16(class);
			GET32(ttl);
			GET16(datalength);
			if (type == TYPE_SOA && class == CLASS_INET) {
				u32 serial, refresh, retry, expire, minimum;
				SKIP_NAME;
				SKIP_NAME;
				GET32(serial);
				GET32(refresh);
				GET32(retry);
				GET32(expire);
				GET32(minimum);
				(void)expire;
				(void)retry;
				(void)refresh;
				(void)serial;
				ttl_r = MIN(ttl_r, ttl);
				ttl_r = MIN(ttl_r, minimum);
			} else {
				/* skip over any other type of resource */
				j += datalength;
			}
		}
	}

	if (ttl_r == 0xffffffff)
		ttl_r = 0;

	reply_handle(req, flags, ttl_r, &reply);
	return 0;
 err:
	if (req)
		reply_handle(req, flags, 0, NULL);
	return -1;
}
