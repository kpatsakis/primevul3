evdns_server_request_add_reply(struct evdns_server_request *req_, int section, const char *name, int type, int class, int ttl, int datalen, int is_name, const char *data)
{
	struct server_request *req = TO_SERVER_REQUEST(req_);
	struct server_reply_item **itemp, *item;
	int *countp;
	int result = -1;

	EVDNS_LOCK(req->port);
	if (req->response) /* have we already answered? */
		goto done;

	switch (section) {
	case EVDNS_ANSWER_SECTION:
		itemp = &req->answer;
		countp = &req->n_answer;
		break;
	case EVDNS_AUTHORITY_SECTION:
		itemp = &req->authority;
		countp = &req->n_authority;
		break;
	case EVDNS_ADDITIONAL_SECTION:
		itemp = &req->additional;
		countp = &req->n_additional;
		break;
	default:
		goto done;
	}
	while (*itemp) {
		itemp = &((*itemp)->next);
	}
	item = mm_malloc(sizeof(struct server_reply_item));
	if (!item)
		goto done;
	item->next = NULL;
	if (!(item->name = mm_strdup(name))) {
		mm_free(item);
		goto done;
	}
	item->type = type;
	item->dns_question_class = class;
	item->ttl = ttl;
	item->is_name = is_name != 0;
	item->datalen = 0;
	item->data = NULL;
	if (data) {
		if (item->is_name) {
			if (!(item->data = mm_strdup(data))) {
				mm_free(item->name);
				mm_free(item);
				goto done;
			}
			item->datalen = (u16)-1;
		} else {
			if (!(item->data = mm_malloc(datalen))) {
				mm_free(item->name);
				mm_free(item);
				goto done;
			}
			item->datalen = datalen;
			memcpy(item->data, data, datalen);
		}
	}

	*itemp = item;
	++(*countp);
	result = 0;
done:
	EVDNS_UNLOCK(req->port);
	return result;
}
