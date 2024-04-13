static void __adjust_nat_entry_set(struct nat_entry_set *nes,
						struct list_head *head, int max)
{
	struct nat_entry_set *cur;

	if (nes->entry_cnt >= max)
		goto add_out;

	list_for_each_entry(cur, head, set_list) {
		if (cur->entry_cnt >= nes->entry_cnt) {
			list_add(&nes->set_list, cur->set_list.prev);
			return;
		}
	}
add_out:
	list_add_tail(&nes->set_list, head);
}
