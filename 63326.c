static int conf__parse_string(char **token, const char *name, char **value, char *saveptr)
{
	*token = strtok_r(NULL, "", &saveptr);
	if(*token){
		if(*value){
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Duplicate %s value in configuration.", name);
			return MOSQ_ERR_INVAL;
		}
		/* Deal with multiple spaces at the beginning of the string. */
		while((*token)[0] == ' ' || (*token)[0] == '\t'){
			(*token)++;
		}
		if(mosquitto_validate_utf8(*token, strlen(*token))){
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Malformed UTF-8 in configuration.");
			return MOSQ_ERR_INVAL;
		}
		*value = mosquitto__strdup(*token);
		if(!*value){
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Out of memory.");
			return MOSQ_ERR_NOMEM;
		}
	}else{
		log__printf(NULL, MOSQ_LOG_ERR, "Error: Empty %s value in configuration.", name);
		return MOSQ_ERR_INVAL;
	}
	return MOSQ_ERR_SUCCESS;
}
