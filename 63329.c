void config__cleanup(struct mosquitto__config *config)
{
	int i;
	int j;

	mosquitto__free(config->clientid_prefixes);
	mosquitto__free(config->persistence_location);
	mosquitto__free(config->persistence_file);
	mosquitto__free(config->persistence_filepath);
	mosquitto__free(config->security_options.auto_id_prefix);
	mosquitto__free(config->security_options.acl_file);
	mosquitto__free(config->security_options.password_file);
	mosquitto__free(config->security_options.psk_file);
	mosquitto__free(config->pid_file);
	if(config->listeners){
		for(i=0; i<config->listener_count; i++){
			mosquitto__free(config->listeners[i].host);
			mosquitto__free(config->listeners[i].mount_point);
			mosquitto__free(config->listeners[i].socks);
			mosquitto__free(config->listeners[i].security_options.auto_id_prefix);
			mosquitto__free(config->listeners[i].security_options.acl_file);
			mosquitto__free(config->listeners[i].security_options.password_file);
			mosquitto__free(config->listeners[i].security_options.psk_file);
#ifdef WITH_TLS
			mosquitto__free(config->listeners[i].cafile);
			mosquitto__free(config->listeners[i].capath);
			mosquitto__free(config->listeners[i].certfile);
			mosquitto__free(config->listeners[i].keyfile);
			mosquitto__free(config->listeners[i].ciphers);
			mosquitto__free(config->listeners[i].psk_hint);
			mosquitto__free(config->listeners[i].crlfile);
			mosquitto__free(config->listeners[i].tls_version);
#ifdef WITH_WEBSOCKETS
			if(!config->listeners[i].ws_context) /* libwebsockets frees its own SSL_CTX */
#endif
			{
				SSL_CTX_free(config->listeners[i].ssl_ctx);
			}
#endif
#ifdef WITH_WEBSOCKETS
			mosquitto__free(config->listeners[i].http_dir);
#endif
		}
		mosquitto__free(config->listeners);
	}
#ifdef WITH_BRIDGE
	if(config->bridges){
		for(i=0; i<config->bridge_count; i++){
			mosquitto__free(config->bridges[i].name);
			if(config->bridges[i].addresses){
				for(j=0; j<config->bridges[i].address_count; j++){
					mosquitto__free(config->bridges[i].addresses[j].address);
				}
				mosquitto__free(config->bridges[i].addresses);
			}
			mosquitto__free(config->bridges[i].remote_clientid);
			mosquitto__free(config->bridges[i].remote_username);
			mosquitto__free(config->bridges[i].remote_password);
			mosquitto__free(config->bridges[i].local_clientid);
			mosquitto__free(config->bridges[i].local_username);
			mosquitto__free(config->bridges[i].local_password);
			if(config->bridges[i].topics){
				for(j=0; j<config->bridges[i].topic_count; j++){
					mosquitto__free(config->bridges[i].topics[j].topic);
					mosquitto__free(config->bridges[i].topics[j].local_prefix);
					mosquitto__free(config->bridges[i].topics[j].remote_prefix);
					mosquitto__free(config->bridges[i].topics[j].local_topic);
					mosquitto__free(config->bridges[i].topics[j].remote_topic);
				}
				mosquitto__free(config->bridges[i].topics);
			}
			mosquitto__free(config->bridges[i].notification_topic);
#ifdef WITH_TLS
			mosquitto__free(config->bridges[i].tls_version);
			mosquitto__free(config->bridges[i].tls_cafile);
#ifdef WITH_TLS_PSK
			mosquitto__free(config->bridges[i].tls_psk_identity);
			mosquitto__free(config->bridges[i].tls_psk);
#endif
#endif
		}
		mosquitto__free(config->bridges);
	}
#endif
	config__cleanup_plugins(config);

	if(config->log_fptr){
		fclose(config->log_fptr);
		config->log_fptr = NULL;
	}
	if(config->log_file){
		mosquitto__free(config->log_file);
		config->log_file = NULL;
	}
}
