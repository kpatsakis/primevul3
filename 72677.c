static void on_gpac_log(void *cbk, GF_LOG_Level ll, GF_LOG_Tool lm, const char *fmt, va_list list)
{
	FILE *logs = cbk ? cbk : stderr;

	if (rti_logs && (lm & GF_LOG_RTI)) {
		char szMsg[2048];
		vsprintf(szMsg, fmt, list);
		UpdateRTInfo(szMsg + 6 /*"[RTI] "*/);
	} else {
		if (log_time_start) {
			u64 now = gf_sys_clock_high_res();
			fprintf(logs, "At "LLD" (diff %d) - ", now - log_time_start, (u32) (now - last_log_time) );
			last_log_time = now;
		}
		if (log_utc_time) {
			u64 utc_clock = gf_net_get_utc() ;
			time_t secs = utc_clock/1000;
			struct tm t;
			t = *gmtime(&secs);
			fprintf(logs, "UTC %d-%02d-%02dT%02d:%02d:%02dZ (TS "LLU") - ", 1900+t.tm_year, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, utc_clock);
		}
		vfprintf(logs, fmt, list);
		fflush(logs);
	}
}
