static inline int bt_index_inc(int index)
{
	return (index + 1) & (BT_WAIT_QUEUES - 1);
}
