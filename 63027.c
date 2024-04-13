person_new(const char* name, spriteset_t* spriteset, bool is_persistent, script_t* create_script)
{
	point3_t  origin = map_origin();
	person_t* person;

	if (++s_num_persons > s_max_persons) {
		s_max_persons = s_num_persons * 2;
		s_persons = realloc(s_persons, s_max_persons * sizeof(person_t*));
	}
	person = s_persons[s_num_persons - 1] = calloc(1, sizeof(person_t));
	person->id = s_next_person_id++;
	person->sprite = spriteset_ref(spriteset);
	set_person_name(person, name);
	person_set_pose(person, spriteset_pose_name(spriteset, 0));
	person->is_persistent = is_persistent;
	person->is_visible = true;
	person->x = origin.x;
	person->y = origin.y;
	person->layer = origin.z;
	person->speed_x = 1.0;
	person->speed_y = 1.0;
	person->anim_frames = spriteset_frame_delay(person->sprite, person->direction, 0);
	person->mask = mk_color(255, 255, 255, 255);
	person->scale_x = person->scale_y = 1.0;
	person->scripts[PERSON_SCRIPT_ON_CREATE] = create_script;
	person_activate(person, PERSON_SCRIPT_ON_CREATE, NULL, true);
	sort_persons();
	return person;
}
