void update_register_server_callbacks(rdpUpdate* update)
{
	update->BeginPaint = update_begin_paint;
	update->EndPaint = update_end_paint;
	update->SetBounds = update_set_bounds;
	update->Synchronize = update_send_synchronize;
	update->DesktopResize = update_send_desktop_resize;
	update->BitmapUpdate = update_send_bitmap_update;
	update->SurfaceBits = update_send_surface_bits;
	update->SurfaceFrameMarker = update_send_surface_frame_marker;
	update->SurfaceCommand = update_send_surface_command;
	update->SurfaceFrameBits = update_send_surface_frame_bits;
	update->PlaySound = update_send_play_sound;
	update->SetKeyboardIndicators = update_send_set_keyboard_indicators;
	update->SetKeyboardImeStatus = update_send_set_keyboard_ime_status;
	update->SaveSessionInfo = rdp_send_save_session_info;
	update->primary->DstBlt = update_send_dstblt;
	update->primary->PatBlt = update_send_patblt;
	update->primary->ScrBlt = update_send_scrblt;
	update->primary->OpaqueRect = update_send_opaque_rect;
	update->primary->LineTo = update_send_line_to;
	update->primary->MemBlt = update_send_memblt;
	update->primary->GlyphIndex = update_send_glyph_index;
	update->secondary->CacheBitmap = update_send_cache_bitmap;
	update->secondary->CacheBitmapV2 = update_send_cache_bitmap_v2;
	update->secondary->CacheBitmapV3 = update_send_cache_bitmap_v3;
	update->secondary->CacheColorTable = update_send_cache_color_table;
	update->secondary->CacheGlyph = update_send_cache_glyph;
	update->secondary->CacheGlyphV2 = update_send_cache_glyph_v2;
	update->secondary->CacheBrush = update_send_cache_brush;
	update->altsec->CreateOffscreenBitmap =
	    update_send_create_offscreen_bitmap_order;
	update->altsec->SwitchSurface = update_send_switch_surface_order;
	update->pointer->PointerSystem = update_send_pointer_system;
	update->pointer->PointerPosition = update_send_pointer_position;
	update->pointer->PointerColor = update_send_pointer_color;
	update->pointer->PointerNew = update_send_pointer_new;
	update->pointer->PointerCached = update_send_pointer_cached;
}
