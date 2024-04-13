static inline void bt_index_atomic_inc(atomic_t *index)
{
	int old = atomic_read(index);
	int new = bt_index_inc(old);
	atomic_cmpxchg(index, old, new);
}
