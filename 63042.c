person_set_spriteset(person_t* person, spriteset_t* spriteset)
{
	spriteset_t* old_spriteset;

	old_spriteset = person->sprite;
	person->sprite = spriteset_ref(spriteset);
	person->anim_frames = spriteset_frame_delay(person->sprite, person->direction, 0);
	person->frame = 0;
	spriteset_unref(old_spriteset);
}
