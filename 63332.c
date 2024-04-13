static void config__init_reload(struct mosquitto_db *db, struct mosquitto__config *config)
{
	int i;
	/* Set defaults */
	for(i=0; i<config->listener_count; i++){
		mosquitto__free(config->listeners[i].security_options.acl_file);
		config->listeners[i].security_options.acl_file = NULL;

		mosquitto__free(config->listeners[i].security_options.password_file);
		config->listeners[i].security_options.password_file = NULL;

		mosquitto__free(config->listeners[i].security_options.psk_file);
		config->listeners[i].security_options.psk_file = NULL;

		config->listeners[i].security_options.allow_anonymous = -1;
		config->listeners[i].security_options.allow_zero_length_clientid = true;
		config->listeners[i].security_options.auto_id_prefix = NULL;
		config->listeners[i].security_options.auto_id_prefix_len = 0;
	}

	config->allow_duplicate_messages = false;

	mosquitto__free(config->security_options.acl_file);
	config->security_options.acl_file = NULL;

	config->security_options.allow_anonymous = -1;
	config->security_options.allow_zero_length_clientid = true;
	config->security_options.auto_id_prefix = NULL;
	config->security_options.auto_id_prefix_len = 0;

	mosquitto__free(config->security_options.password_file);
	config->security_options.password_file = NULL;

	mosquitto__free(config->security_options.psk_file);
	config->security_options.psk_file = NULL;

	config->autosave_interval = 1800;
	config->autosave_on_changes = false;
	mosquitto__free(config->clientid_prefixes);
	config->connection_messages = true;
	config->clientid_prefixes = NULL;
	config->per_listener_settings = false;
	if(config->log_fptr){
		fclose(config->log_fptr);
		config->log_fptr = NULL;
	}
	mosquitto__free(config->log_file);
	config->log_file = NULL;

#if defined(WIN32) || defined(__CYGWIN__)
	if(service_handle){
		/* This is running as a Windows service. Default to no logging. Using
		 * stdout/stderr is forbidden because the first clients to connect will
		 * get log information sent to them for some reason. */
		config->log_dest = MQTT3_LOG_NONE;
	}else{
		config->log_dest = MQTT3_LOG_STDERR;
	}
#else
	config->log_facility = LOG_DAEMON;
	config->log_dest = MQTT3_LOG_STDERR;
	if(db->verbose){
		config->log_type = INT_MAX;
	}else{
		config->log_type = MOSQ_LOG_ERR | MOSQ_LOG_WARNING | MOSQ_LOG_NOTICE | MOSQ_LOG_INFO;
	}
#endif
	config->log_timestamp = true;
	config->persistence = false;
	mosquitto__free(config->persistence_location);
	config->persistence_location = NULL;
	mosquitto__free(config->persistence_file);
	config->persistence_file = NULL;
	config->persistent_client_expiration = 0;
	config->queue_qos0_messages = false;
	config->set_tcp_nodelay = false;
	config->sys_interval = 10;
	config->upgrade_outgoing_qos = false;

	config__cleanup_plugins(config);
}
