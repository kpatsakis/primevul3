static int cmp_extents_forward(const void *a, const void *b)
{
	const struct uid_gid_extent *e1 = a;
	const struct uid_gid_extent *e2 = b;

	if (e1->first < e2->first)
		return -1;

	if (e1->first > e2->first)
		return 1;

	return 0;
}
