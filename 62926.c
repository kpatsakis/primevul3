command_person(person_t* person, int command)
{
	double    new_x;
	double    new_y;
	person_t* person_to_touch;

	new_x = person->x;
	new_y = person->y;
	switch (command) {
	case COMMAND_ANIMATE:
		person->revert_frames = person->revert_delay;
		if (person->anim_frames > 0 && --person->anim_frames == 0) {
			++person->frame;
			person->anim_frames = spriteset_frame_delay(person->sprite, person->direction, person->frame);
		}
		break;
	case COMMAND_FACE_NORTH:
		person_set_pose(person, "north");
		break;
	case COMMAND_FACE_NORTHEAST:
		person_set_pose(person, "northeast");
		break;
	case COMMAND_FACE_EAST:
		person_set_pose(person, "east");
		break;
	case COMMAND_FACE_SOUTHEAST:
		person_set_pose(person, "southeast");
		break;
	case COMMAND_FACE_SOUTH:
		person_set_pose(person, "south");
		break;
	case COMMAND_FACE_SOUTHWEST:
		person_set_pose(person, "southwest");
		break;
	case COMMAND_FACE_WEST:
		person_set_pose(person, "west");
		break;
	case COMMAND_FACE_NORTHWEST:
		person_set_pose(person, "northwest");
		break;
	case COMMAND_MOVE_NORTH:
		new_y = person->y - person->speed_y;
		break;
	case COMMAND_MOVE_EAST:
		new_x = person->x + person->speed_x;
		break;
	case COMMAND_MOVE_SOUTH:
		new_y = person->y + person->speed_y;
		break;
	case COMMAND_MOVE_WEST:
		new_x = person->x - person->speed_x;
		break;
	}
	if (new_x != person->x || new_y != person->y) {
		if (!person_obstructed_at(person, new_x, new_y, &person_to_touch, NULL)) {
			if (new_x != person->x)
				person->mv_x = new_x > person->x ? 1 : -1;
			if (new_y != person->y)
				person->mv_y = new_y > person->y ? 1 : -1;
			person->x = new_x;
			person->y = new_y;
		}
		else {
			if (person_to_touch != NULL)
				person_activate(person_to_touch, PERSON_SCRIPT_ON_TOUCH, person, true);
		}
	}
}
