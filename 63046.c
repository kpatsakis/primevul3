person_talk(const person_t* person)
{
	rect_t          map_rect;
	person_t*       target_person;
	double          talk_x, talk_y;

	map_rect = map_bounds();

	person_get_xy(person, &talk_x, &talk_y, true);
	if (strstr(person->direction, "north"))
		talk_y -= s_talk_distance;
	if (strstr(person->direction, "east"))
		talk_x += s_talk_distance;
	if (strstr(person->direction, "south"))
		talk_y += s_talk_distance;
	if (strstr(person->direction, "west"))
		talk_x -= s_talk_distance;
	person_obstructed_at(person, talk_x, talk_y, &target_person, NULL);

	if (target_person != NULL)
		person_activate(target_person, PERSON_SCRIPT_ON_TALK, person, true);
}
