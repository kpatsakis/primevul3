static int fsck_msg_type(enum fsck_msg_id msg_id,
	struct fsck_options *options)
{
	int msg_type;

	assert(msg_id >= 0 && msg_id < FSCK_MSG_MAX);

	if (options->msg_type)
		msg_type = options->msg_type[msg_id];
	else {
		msg_type = msg_id_info[msg_id].msg_type;
		if (options->strict && msg_type == FSCK_WARN)
			msg_type = FSCK_ERROR;
	}

	return msg_type;
}
