static void loop_remove(struct loop_device *lo)
{
	blk_cleanup_queue(lo->lo_queue);
	del_gendisk(lo->lo_disk);
	blk_mq_free_tag_set(&lo->tag_set);
	put_disk(lo->lo_disk);
	kfree(lo);
}
