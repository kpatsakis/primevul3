draw_persons(int layer, bool is_flipped, int cam_x, int cam_y)
{
	person_t*    person;
	spriteset_t* sprite;
	int          w, h;
	double       x, y;
	int          i;

	for (i = 0; i < s_num_persons; ++i) {
		person = s_persons[i];
		if (!person->is_visible || person->layer != layer)
			continue;
		sprite = person->sprite;
		w = spriteset_width(sprite);
		h = spriteset_height(sprite);
		person_get_xy(person, &x, &y, true);
		x -= cam_x - person->x_offset;
		y -= cam_y - person->y_offset;
		spriteset_draw(sprite, person->mask, is_flipped, person->theta, person->scale_x, person->scale_y,
			person->direction, trunc(x), trunc(y), person->frame);
	}
}
