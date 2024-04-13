newSubscriber(const char * eventurl, const char * callback, int callbacklen)
{
	struct subscriber * tmp;
	if(!eventurl || !callback || !callbacklen)
		return NULL;
	tmp = calloc(1, sizeof(struct subscriber)+callbacklen+1);
	if(!tmp)
		return NULL;
	if(strcmp(eventurl, WANCFG_EVENTURL)==0)
		tmp->service = EWanCFG;
	else if(strcmp(eventurl, WANIPC_EVENTURL)==0)
		tmp->service = EWanIPC;
#ifdef ENABLE_L3F_SERVICE
	else if(strcmp(eventurl, L3F_EVENTURL)==0)
		tmp->service = EL3F;
#endif
#ifdef ENABLE_6FC_SERVICE
	else if(strcmp(eventurl, WANIP6FC_EVENTURL)==0)
		tmp->service = E6FC;
#endif
#ifdef ENABLE_DP_SERVICE
	else if(strcmp(eventurl, DP_EVENTURL)==0)
		tmp->service = EDP;
#endif
	else {
		free(tmp);
		return NULL;
	}
	memcpy(tmp->callback, callback, callbacklen);
	tmp->callback[callbacklen] = '\0';
#if defined(LIB_UUID)
	{
		uuid_t uuid;
		uuid_generate(uuid);
		memcpy(tmp->uuid, "uuid:", 5);
		uuid_unparse(uuid, tmp->uuid + 5);
	}
#elif defined(BSD_UUID)
	{
		uuid_t uuid;
		uint32_t status;
		uuid_create(&uuid, &status);
		if(status != uuid_s_ok) {
			syslog(LOG_ERR, "uuid_create() failed (%u)", status);
		} else {
			char * uuid_str;
			uuid_to_string(&uuid, &uuid_str, &status);
			if(status != uuid_s_ok) {
				syslog(LOG_ERR, "uuid_to_string() failed (%u)", status);
			} else {
				if(strlen(uuid_str) != 36) {
					syslog(LOG_ERR, "uuid_to_string() returned %s", uuid_str);
					status = (uint32_t)-1;
				} else {
					memcpy(tmp->uuid, "uuid:", 5);
					memcpy(tmp->uuid + 5, uuid_str, 36);
					tmp->uuid[sizeof(tmp->uuid)-1] = '\0';
				}
				free(uuid_str);
			}
		}
		if(status != uuid_s_ok) {
			/* make a dummy uuid */
			strncpy(tmp->uuid, uuidvalue_igd, sizeof(tmp->uuid));
			tmp->uuid[sizeof(tmp->uuid)-1] = '\0';
			snprintf(tmp->uuid+sizeof(tmp->uuid)-5, 5, "%04lx", random() & 0xffff);
		}
	}
#else
	/* make a dummy uuid */
	strncpy(tmp->uuid, uuidvalue_igd, sizeof(tmp->uuid));
	tmp->uuid[sizeof(tmp->uuid)-1] = '\0';
	snprintf(tmp->uuid+sizeof(tmp->uuid)-5, 5, "%04lx", random() & 0xffff);
#endif
	return tmp;
}
