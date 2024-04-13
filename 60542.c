evutil_weakrand_range_(struct evutil_weakrand_state *state, ev_int32_t top)
{
	ev_int32_t divisor, result;

	/* We can't just do weakrand() % top, since the low bits of the LCG
	 * are less random than the high ones.  (Specifically, since the LCG
	 * modulus is 2^N, every 2^m for m<N will divide the modulus, and so
	 * therefore the low m bits of the LCG will have period 2^m.) */
	divisor = EVUTIL_WEAKRAND_MAX / top;
	do {
		result = evutil_weakrand_(state) / divisor;
	} while (result >= top);
	return result;
}
