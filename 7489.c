smb2_get_credits(struct mid_q_entry *mid)
{
	return mid->credits_received;
}