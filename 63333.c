int config__read(struct mosquitto_db *db, struct mosquitto__config *config, bool reload)
{
	int rc = MOSQ_ERR_SUCCESS;
	struct config_recurse cr;
	int lineno = 0;
	int len;
	struct mosquitto__config config_reload;
	int i;

	if(reload){
		memset(&config_reload, 0, sizeof(struct mosquitto__config));
	}

	cr.log_dest = MQTT3_LOG_NONE;
	cr.log_dest_set = 0;
	cr.log_type = MOSQ_LOG_NONE;
	cr.log_type_set = 0;
	cr.max_inflight_bytes = 0;
	cr.max_inflight_messages = 20;
	cr.max_queued_bytes = 0;
	cr.max_queued_messages = 100;

	if(!db->config_file) return 0;

	if(reload){
		/* Re-initialise appropriate config vars to default for reload. */
		config__init_reload(db, &config_reload);
		config_reload.listeners = config->listeners;
		config_reload.listener_count = config->listener_count;
		rc = config__read_file(&config_reload, reload, db->config_file, &cr, 0, &lineno);
	}else{
		rc = config__read_file(config, reload, db->config_file, &cr, 0, &lineno);
	}
	if(rc){
		log__printf(NULL, MOSQ_LOG_ERR, "Error found at %s:%d.", db->config_file, lineno);
		return rc;
	}

	if(reload){
		config__copy(&config_reload, config);
	}

	/* If auth/access options are set and allow_anonymous not explicitly set, disallow anon. */
	if(config->per_listener_settings){
		for(i=0; i<config->listener_count; i++){
			if(config->listeners[i].security_options.allow_anonymous == -1){
				if(config->listeners[i].security_options.password_file
					|| config->listeners[i].security_options.psk_file
					|| config->listeners[i].security_options.auth_plugin_configs){

					/* allow_anonymous not set explicitly, some other security options
					* have been set - so disable allow_anonymous
					*/
					config->listeners[i].security_options.allow_anonymous = false;
				}else{
					/* Default option if no security options set */
					config->listeners[i].security_options.allow_anonymous = true;
				}
			}
		}
	}else{
		if(config->security_options.allow_anonymous == -1){
			if(config->security_options.password_file
				 || config->security_options.psk_file
				 || config->security_options.auth_plugin_configs){

				/* allow_anonymous not set explicitly, some other security options
				* have been set - so disable allow_anonymous
				*/
				config->security_options.allow_anonymous = false;
			}else{
				/* Default option if no security options set */
				config->security_options.allow_anonymous = true;
			}
		}
	}
#ifdef WITH_PERSISTENCE
	if(config->persistence){
		if(!config->persistence_file){
			config->persistence_file = mosquitto__strdup("mosquitto.db");
			if(!config->persistence_file) return MOSQ_ERR_NOMEM;
		}
		mosquitto__free(config->persistence_filepath);
		if(config->persistence_location && strlen(config->persistence_location)){
			len = strlen(config->persistence_location) + strlen(config->persistence_file) + 1;
			config->persistence_filepath = mosquitto__malloc(len);
			if(!config->persistence_filepath) return MOSQ_ERR_NOMEM;
			snprintf(config->persistence_filepath, len, "%s%s", config->persistence_location, config->persistence_file);
		}else{
			config->persistence_filepath = mosquitto__strdup(config->persistence_file);
			if(!config->persistence_filepath) return MOSQ_ERR_NOMEM;
		}
	}
#endif
	/* Default to drop to mosquitto user if no other user specified. This must
	 * remain here even though it is covered in config__parse_args() because this
	 * function may be called on its own. */
	if(!config->user){
		config->user = "mosquitto";
	}

	db__limits_set(cr.max_inflight_messages, cr.max_inflight_bytes, cr.max_queued_messages, cr.max_queued_bytes);

#ifdef WITH_BRIDGE
	for(i=0; i<config->bridge_count; i++){
		if(!config->bridges[i].name || !config->bridges[i].addresses || !config->bridges[i].topic_count){
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Invalid bridge configuration.");
			return MOSQ_ERR_INVAL;
		}
#ifdef WITH_TLS_PSK
		if(config->bridges[i].tls_psk && !config->bridges[i].tls_psk_identity){
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Invalid bridge configuration: missing bridge_identity.\n");
			return MOSQ_ERR_INVAL;
		}
		if(config->bridges[i].tls_psk_identity && !config->bridges[i].tls_psk){
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Invalid bridge configuration: missing bridge_psk.\n");
			return MOSQ_ERR_INVAL;
		}
#endif
	}
#endif

	if(cr.log_dest_set){
		config->log_dest = cr.log_dest;
	}
	if(db->verbose){
		config->log_type = INT_MAX;
	}else if(cr.log_type_set){
		config->log_type = cr.log_type;
	}
	return MOSQ_ERR_SUCCESS;
}
