static void blk_mq_del_queue_tag_set(struct request_queue *q)
{
	struct blk_mq_tag_set *set = q->tag_set;

	mutex_lock(&set->tag_list_lock);
	list_del_init(&q->tag_set_list);
	blk_mq_update_tag_set_depth(set);
	mutex_unlock(&set->tag_list_lock);
}
