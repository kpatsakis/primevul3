person_set_frame_delay(person_t* person, int num_frames)
{
	person->anim_frames = num_frames;
	person->revert_frames = person->revert_delay;
}
