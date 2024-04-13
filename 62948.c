map_add_zone(rect_t bounds, int layer, script_t* script, int steps)
{
	struct map_zone zone;

	console_log(2, "creating %u-step zone #%d on map '%s'", steps, vector_len(s_map->zones), s_map_filename);
	console_log(3, "    bounds: (%d,%d)-(%d,%d)", bounds.x1, bounds.y1, bounds.x2, bounds.y2);

	memset(&zone, 0, sizeof(struct map_zone));
	zone.bounds = bounds;
	zone.layer = layer;
	zone.script = script_ref(script);
	zone.interval = steps;
	zone.steps_left = 0;
	if (!vector_push(s_map->zones, &zone))
		return false;
	return true;
}
