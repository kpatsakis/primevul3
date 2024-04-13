static void etm_free_aux(void *data)
{
	struct etm_event_data *event_data = data;

	schedule_work(&event_data->work);
}
