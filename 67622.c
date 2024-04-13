void update_free(rdpUpdate* update)
{
	if (update != NULL)
	{
		OFFSCREEN_DELETE_LIST* deleteList = &(update->altsec->create_offscreen_bitmap.deleteList);

		if (deleteList)
			free(deleteList->indices);

		free(update->pointer);

		if (update->primary)
		{
			free(update->primary->polyline.points);
			free(update->primary->polygon_sc.points);
			free(update->primary->fast_glyph.glyphData.aj);
			free(update->primary);
		}

		free(update->secondary);
		free(update->altsec);

		if (update->window)
		{
			free(update->window->monitored_desktop.windowIds);
			update_free_window_state(&update->window->window_state);
			update_free_window_icon_info(update->window->window_icon.iconInfo);
			free(update->window);
		}

		MessageQueue_Free(update->queue);
		free(update);
	}
}
