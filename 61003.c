static int stream_thunk(void *buf, size_t size, void *data)
{
	int error = 0;
	struct push_packbuilder_payload *payload = data;

	if ((error = payload->stream->write(payload->stream, (const char *)buf, size)) < 0)
		return error;

	if (payload->cb) {
		double current_time = git__timer();
		payload->last_bytes += size;

		if ((current_time - payload->last_progress_report_time) >= MIN_PROGRESS_UPDATE_INTERVAL) {
			payload->last_progress_report_time = current_time;
			error = payload->cb(payload->pb->nr_written, payload->pb->nr_objects, payload->last_bytes, payload->cb_payload);
		}
	}

	return error;
}
