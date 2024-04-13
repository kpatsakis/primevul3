evdns_request_remove(struct request *req, struct request **head)
{
	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);

#if 0
	{
		struct request *ptr;
		int found = 0;
		EVUTIL_ASSERT(*head != NULL);

		ptr = *head;
		do {
			if (ptr == req) {
				found = 1;
				break;
			}
			ptr = ptr->next;
		} while (ptr != *head);
		EVUTIL_ASSERT(found);

		EVUTIL_ASSERT(req->next);
	}
#endif

	if (req->next == req) {
		/* only item in the list */
		*head = NULL;
	} else {
		req->next->prev = req->prev;
		req->prev->next = req->next;
		if (*head == req) *head = req->next;
	}
	req->next = req->prev = NULL;
}
