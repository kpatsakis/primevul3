person_get_xy(const person_t* person, double* out_x, double* out_y, bool normalize)
{
	*out_x = person->x;
	*out_y = person->y;
	if (normalize)
		map_normalize_xy(out_x, out_y, person->layer);
}
