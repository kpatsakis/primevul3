person_get_frame(const person_t* person)
{
	int num_frames;

	num_frames = spriteset_num_frames(person->sprite, person->direction);
	return person->frame % num_frames;
}
