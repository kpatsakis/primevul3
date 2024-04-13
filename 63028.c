person_obstructed_at(const person_t* person, double x, double y, person_t** out_obstructing_person, int* out_tile_index)
{
	rect_t           area;
	rect_t           base, my_base;
	double           cur_x, cur_y;
	bool             is_obstructed = false;
	int              layer;
	const obsmap_t*  obsmap;
	int              tile_w, tile_h;
	const tileset_t* tileset;

	int i, i_x, i_y;

	map_normalize_xy(&x, &y, person->layer);
	person_get_xyz(person, &cur_x, &cur_y, &layer, true);
	my_base = rect_translate(person_base(person), x - cur_x, y - cur_y);
	if (out_obstructing_person != NULL)
		*out_obstructing_person = NULL;
	if (out_tile_index != NULL)
		*out_tile_index = -1;

	if (!person->ignore_all_persons) {
		for (i = 0; i < s_num_persons; ++i) {
			if (s_persons[i] == person)  // these persons aren't going to obstruct themselves!
				continue;
			if (s_persons[i]->layer != layer)
				continue;  // ignore persons not on the same layer
			if (person_following(s_persons[i], person))
				continue;  // ignore own followers
			base = person_base(s_persons[i]);
			if (do_rects_overlap(my_base, base) && !person_ignored_by(person, s_persons[i])) {
				is_obstructed = true;
				if (out_obstructing_person)
					*out_obstructing_person = s_persons[i];
				break;
			}
		}
	}

	obsmap = layer_obsmap(layer);
	if (obsmap_test_rect(obsmap, my_base))
		is_obstructed = true;

	if (!person->ignore_all_tiles) {
		tileset = map_tileset();
		tileset_get_size(tileset, &tile_w, &tile_h);
		area.x1 = my_base.x1 / tile_w;
		area.y1 = my_base.y1 / tile_h;
		area.x2 = area.x1 + (my_base.x2 - my_base.x1) / tile_w + 2;
		area.y2 = area.y1 + (my_base.y2 - my_base.y1) / tile_h + 2;
		for (i_x = area.x1; i_x < area.x2; ++i_x) for (i_y = area.y1; i_y < area.y2; ++i_y) {
			base = rect_translate(my_base, -(i_x * tile_w), -(i_y * tile_h));
			obsmap = tileset_obsmap(tileset, map_tile_at(i_x, i_y, layer));
			if (obsmap != NULL && obsmap_test_rect(obsmap, base)) {
				is_obstructed = true;
				if (out_tile_index)
					*out_tile_index = map_tile_at(i_x, i_y, layer);
				break;
			}
		}
	}

	return is_obstructed;
}
