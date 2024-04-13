evdns_request_insert(struct request *req, struct request **head) {
	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);
	if (!*head) {
		*head = req;
		req->next = req->prev = req;
		return;
	}

	req->prev = (*head)->prev;
	req->prev->next = req;
	req->next = *head;
	(*head)->prev = req;
}
