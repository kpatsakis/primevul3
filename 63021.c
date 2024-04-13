person_has_moved(const person_t* person)
{
	return person->mv_x != 0 || person->mv_y != 0;
}
