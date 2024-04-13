route4_reset_fastmap(struct route4_head *head)
{
	spin_lock_bh(&fastmap_lock);
	memset(head->fastmap, 0, sizeof(head->fastmap));
	spin_unlock_bh(&fastmap_lock);
}