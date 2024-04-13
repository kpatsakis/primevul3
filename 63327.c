static void conf__set_cur_security_options(struct mosquitto__config *config, struct mosquitto__listener *cur_listener, struct mosquitto__security_options **security_options)
{
	if(config->per_listener_settings){
		(*security_options) = &cur_listener->security_options;
	}else{
		(*security_options) = &config->security_options;
	}
}
