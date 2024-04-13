static void free_request_size(void *element, void *data)
{
	struct request_queue *q = data;

	if (q->exit_rq_fn)
		q->exit_rq_fn(q, element);
	kfree(element);
}
