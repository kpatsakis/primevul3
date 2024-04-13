rdpsnd_queue_write(STREAM s, uint16 tick, uint8 index)
{
	struct audio_packet *packet = &packet_queue[queue_hi];
	unsigned int next_hi = (queue_hi + 1) % MAX_QUEUE;

	if (next_hi == queue_pending)
	{
		logger(Sound, Error, "rdpsnd_queue_write(), no space to queue audio packet");
		return;
	}

	queue_hi = next_hi;

	packet->s = *s;
	packet->tick = tick;
	packet->index = index;

	gettimeofday(&packet->arrive_tv, NULL);
}
