void config__copy(struct mosquitto__config *src, struct mosquitto__config *dest)
{
	mosquitto__free(dest->security_options.acl_file);
	dest->security_options.acl_file = src->security_options.acl_file;

	dest->security_options.allow_anonymous = src->security_options.allow_anonymous;
	dest->security_options.allow_zero_length_clientid = src->security_options.allow_zero_length_clientid;

	mosquitto__free(dest->security_options.auto_id_prefix);
	dest->security_options.auto_id_prefix = src->security_options.auto_id_prefix;
	dest->security_options.auto_id_prefix_len = src->security_options.auto_id_prefix_len;

	mosquitto__free(dest->security_options.password_file);
	dest->security_options.password_file = src->security_options.password_file;

	mosquitto__free(dest->security_options.psk_file);
	dest->security_options.psk_file = src->security_options.psk_file;


	dest->allow_duplicate_messages = src->allow_duplicate_messages;


	dest->autosave_interval = src->autosave_interval;
	dest->autosave_on_changes = src->autosave_on_changes;

	mosquitto__free(dest->clientid_prefixes);
	dest->clientid_prefixes = src->clientid_prefixes;

	dest->connection_messages = src->connection_messages;
	dest->log_dest = src->log_dest;
	dest->log_facility = src->log_facility;
	dest->log_type = src->log_type;
	dest->log_timestamp = src->log_timestamp;

	mosquitto__free(dest->log_file);
	dest->log_file = src->log_file;

	dest->message_size_limit = src->message_size_limit;

	dest->persistence = src->persistence;

	mosquitto__free(dest->persistence_location);
	dest->persistence_location = src->persistence_location;

	mosquitto__free(dest->persistence_file);
	dest->persistence_file = src->persistence_file;

	mosquitto__free(dest->persistence_filepath);
	dest->persistence_filepath = src->persistence_filepath;

	dest->persistent_client_expiration = src->persistent_client_expiration;


	dest->queue_qos0_messages = src->queue_qos0_messages;
	dest->sys_interval = src->sys_interval;
	dest->upgrade_outgoing_qos = src->upgrade_outgoing_qos;

#ifdef WITH_WEBSOCKETS
	dest->websockets_log_level = src->websockets_log_level;
#endif
}
