char EVUTIL_TOUPPER_(char c)
{
	return ((char)EVUTIL_TOUPPER_TABLE[(ev_uint8_t)c]);
}
