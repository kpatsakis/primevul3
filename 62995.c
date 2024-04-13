map_xy_from_screen(point2_t screen_xy)
{
	int x;
	int y;

	x = screen_xy.x;
	y = screen_xy.y;
	map_screen_to_map(s_camera_x, s_camera_y, &x, &y);
	return mk_point2(x, y);
}
