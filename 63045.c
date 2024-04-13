person_set_xyz(person_t* person, double x, double y, int layer)
{
	person->x = x;
	person->y = y;
	person->layer = layer;
	sort_persons();
}
