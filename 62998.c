person_base(const person_t* person)
{
	rect_t base_rect;
	int    base_x;
	int    base_y;
	double x;
	double y;

	base_rect = rect_zoom(spriteset_get_base(person->sprite), person->scale_x, person->scale_y);
	person_get_xy(person, &x, &y, true);
	base_x = x - (base_rect.x1 + (base_rect.x2 - base_rect.x1) / 2);
	base_y = y - (base_rect.y1 + (base_rect.y2 - base_rect.y1) / 2);
	base_rect.x1 += base_x; base_rect.x2 += base_x;
	base_rect.y1 += base_y; base_rect.y2 += base_y;
	return base_rect;
}
