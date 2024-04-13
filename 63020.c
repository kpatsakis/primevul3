person_get_xyz(const person_t* person, double* out_x, double* out_y, int* out_layer, bool normalize)
{
	*out_x = person->x;
	*out_y = person->y;
	*out_layer = person->layer;
	if (normalize)
		map_normalize_xy(out_x, out_y, *out_layer);
}
