void config__init(struct mosquitto_db *db, struct mosquitto__config *config)
{
	memset(config, 0, sizeof(struct mosquitto__config));
	config__init_reload(db, config);

	config->daemon = false;
	memset(&config->default_listener, 0, sizeof(struct mosquitto__listener));
	config->default_listener.max_connections = -1;
	config->default_listener.protocol = mp_mqtt;
	config->default_listener.security_options.allow_anonymous = -1;
}
