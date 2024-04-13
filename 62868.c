append_id_w(wchar_t **wp, int id)
{
	if (id < 0)
		id = 0;
	if (id > 9)
		append_id_w(wp, id / 10);
	*(*wp)++ = L"0123456789"[id % 10];
}
