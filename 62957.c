map_engine_draw_map(void)
{
	bool              is_repeating;
	int               cell_x;
	int               cell_y;
	int               first_cell_x;
	int               first_cell_y;
	struct map_layer* layer;
	int               layer_height;
	int               layer_width;
	size2_t           resolution;
	int               tile_height;
	int               tile_index;
	int               tile_width;
	int               off_x;
	int               off_y;

	int x, y, z;

	if (screen_skipping_frame(g_screen))
		return;

	resolution = screen_size(g_screen);
	tileset_get_size(s_map->tileset, &tile_width, &tile_height);

	for (z = 0; z < s_map->num_layers; ++z) {
		layer = &s_map->layers[z];
		is_repeating = s_map->is_repeating || layer->is_parallax;
		layer_width = layer->width * tile_width;
		layer_height = layer->height * tile_height;
		off_x = 0;
		off_y = 0;
		map_screen_to_layer(z, s_camera_x, s_camera_y, &off_x, &off_y);

		al_hold_bitmap_drawing(true);
		if (layer->is_reflective) {
			if (is_repeating) {  // for small repeating maps, persons need to be repeated as well
				for (y = 0; y < resolution.height / layer_height + 2; ++y) for (x = 0; x < resolution.width / layer_width + 2; ++x)
					draw_persons(z, true, off_x - x * layer_width, off_y - y * layer_height);
			}
			else {
				draw_persons(z, true, off_x, off_y);
			}
		}

		if (layer->is_visible) {
			first_cell_x = off_x / tile_width;
			first_cell_y = off_y / tile_height;
			for (y = 0; y < resolution.height / tile_height + 2; ++y) for (x = 0; x < resolution.width / tile_width + 2; ++x) {
				cell_x = is_repeating ? (x + first_cell_x) % layer->width : x + first_cell_x;
				cell_y = is_repeating ? (y + first_cell_y) % layer->height : y + first_cell_y;
				if (cell_x < 0 || cell_x >= layer->width || cell_y < 0 || cell_y >= layer->height)
					continue;
				tile_index = layer->tilemap[cell_x + cell_y * layer->width].tile_index;
				tileset_draw(s_map->tileset, layer->color_mask, x * tile_width - off_x % tile_width, y * tile_height - off_y % tile_height, tile_index);
			}
		}

		if (is_repeating) {  // for small repeating maps, persons need to be repeated as well
			for (y = 0; y < resolution.height / layer_height + 2; ++y) for (x = 0; x < resolution.width / layer_width + 2; ++x)
				draw_persons(z, false, off_x - x * layer_width, off_y - y * layer_height);
		}
		else {
			draw_persons(z, false, off_x, off_y);
		}
		al_hold_bitmap_drawing(false);

		script_run(layer->render_script, false);
	}

	al_draw_filled_rectangle(0, 0, resolution.width, resolution.height, nativecolor(s_color_mask));
	script_run(s_render_script, false);
}
