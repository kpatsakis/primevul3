person_set_revert_delay(person_t* person, int num_frames)
{
	person->revert_delay = num_frames;
	person->revert_frames = num_frames;
}
