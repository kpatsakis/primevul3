static void blk_mq_add_queue_tag_set(struct blk_mq_tag_set *set,
				     struct request_queue *q)
{
	q->tag_set = set;

	mutex_lock(&set->tag_list_lock);
	list_add_tail(&q->tag_set_list, &set->tag_list);
	blk_mq_update_tag_set_depth(set);
	mutex_unlock(&set->tag_list_lock);
}
