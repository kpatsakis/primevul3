trigger_get_xyz(int trigger_index, int* out_x, int* out_y, int* out_layer)
{
	struct map_trigger* trigger;

	trigger = vector_get(s_map->triggers, trigger_index);
	if (out_x != NULL)
		*out_x = trigger->x;
	if (out_y != NULL)
		*out_y = trigger->y;
	if (out_layer) *out_layer = trigger->z;
}
