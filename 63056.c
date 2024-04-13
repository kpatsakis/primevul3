update_person(person_t* person, bool* out_has_moved)
{
	struct command  command;
	double          delta_x, delta_y;
	int             facing;
	bool            has_moved;
	bool            is_finished;
	const person_t* last_person;
	struct step     step;
	int             vector;

	int i;

	person->mv_x = 0; person->mv_y = 0;
	if (person->revert_frames > 0 && --person->revert_frames <= 0)
		person->frame = 0;
	if (person->leader == NULL) {  // no leader; use command queue
		if (person->num_commands == 0)
			person_activate(person, PERSON_SCRIPT_GENERATOR, NULL, true);

		is_finished = !does_person_exist(person) || person->num_commands == 0;
		while (!is_finished) {
			command = person->commands[0];
			--person->num_commands;
			for (i = 0; i < person->num_commands; ++i)
				person->commands[i] = person->commands[i + 1];
			last_person = s_current_person;
			s_current_person = person;
			if (command.type != COMMAND_RUN_SCRIPT)
				command_person(person, command.type);
			else
				script_run(command.script, false);
			s_current_person = last_person;
			script_unref(command.script);
			is_finished = !does_person_exist(person)  // stop if person was destroyed
				|| !command.is_immediate || person->num_commands == 0;
		}
	}
	else {  // leader set; follow the leader!
		step = person->leader->steps[person->follow_distance - 1];
		delta_x = step.x - person->x;
		delta_y = step.y - person->y;
		if (fabs(delta_x) > person->speed_x)
			command_person(person, delta_x > 0 ? COMMAND_MOVE_EAST : COMMAND_MOVE_WEST);
		if (!does_person_exist(person)) return;
		if (fabs(delta_y) > person->speed_y)
			command_person(person, delta_y > 0 ? COMMAND_MOVE_SOUTH : COMMAND_MOVE_NORTH);
		if (!does_person_exist(person)) return;
		vector = person->mv_x + person->mv_y * 3;
		facing = vector == -3 ? COMMAND_FACE_NORTH
			: vector == -2 ? COMMAND_FACE_NORTHEAST
			: vector == 1 ? COMMAND_FACE_EAST
			: vector == 4 ? COMMAND_FACE_SOUTHEAST
			: vector == 3 ? COMMAND_FACE_SOUTH
			: vector == 2 ? COMMAND_FACE_SOUTHWEST
			: vector == -1 ? COMMAND_FACE_WEST
			: vector == -4 ? COMMAND_FACE_NORTHWEST
			: COMMAND_WAIT;
		if (facing != COMMAND_WAIT)
			command_person(person, COMMAND_ANIMATE);
		if (!does_person_exist(person)) return;
		command_person(person, facing);
	}

	if (!does_person_exist(person))
		return;  // they probably got eaten by a pig.

	*out_has_moved = person_has_moved(person);
	if (*out_has_moved)
		record_step(person);

	for (i = 0; i < s_num_persons; ++i) {
		if (s_persons[i]->leader != person)
			continue;
		update_person(s_persons[i], &has_moved);
		*out_has_moved |= has_moved;
	}
}
