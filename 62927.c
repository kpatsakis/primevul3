compare_persons(const void* a, const void* b)
{
	person_t* p1 = *(person_t**)a;
	person_t* p2 = *(person_t**)b;

	double x, y_p1, y_p2;
	int    y_delta;

	person_get_xy(p1, &x, &y_p1, true);
	person_get_xy(p2, &x, &y_p2, true);
	y_delta = y_p1 - y_p2;
	if (y_delta != 0)
		return y_delta;
	else if (person_following(p1, p2))
		return -1;
	else if (person_following(p2, p1))
		return 1;
	else
		return p1->id - p2->id;
}
