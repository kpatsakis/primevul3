person_get_scale(const person_t* person, double* out_scale_x, double* out_scale_y)
{
	*out_scale_x = person->scale_x;
	*out_scale_y = person->scale_y;
}
