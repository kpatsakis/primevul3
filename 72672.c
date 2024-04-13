static Bool get_time_list(char *arg, u32 *times, u32 *nb_times)
{
	char *str;
	Float var;
	Double sec;
	u32 h, m, s, ms, f, fps;
	if (!arg || (arg[0]=='-') || !isdigit(arg[0])) return 0;

	/*SMPTE time code*/
	if (strchr(arg, ':') && strchr(arg, ';') && strchr(arg, '/')) {
		if (sscanf(arg, "%02ud:%02ud:%02ud;%02ud/%02ud", &h, &m, &s, &f, &fps)==5) {
			sec = 0;
			if (fps) sec = ((Double)f) / fps;
			sec += 3600*h + 60*m + s;
			times[*nb_times] = (u32) (1000*sec);
			(*nb_times) ++;
			return 1;
		}
	}
	while (arg) {
		str = strchr(arg, '-');
		if (str) str[0] = 0;
		/*HH:MM:SS:MS time code*/
		if (strchr(arg, ':') && (sscanf(arg, "%u:%u:%u:%u", &h, &m, &s, &ms)==4)) {
			sec = ms;
			sec /= 1000;
			sec += 3600*h + 60*m + s;
			times[*nb_times] = (u32) (1000*sec);
			(*nb_times) ++;
		} else if (sscanf(arg, "%f", &var)==1) {
			sec = atof(arg);
			times[*nb_times] = (u32) (1000*sec);
			(*nb_times) ++;
		}
		if (!str) break;
		str[0] = '-';
		arg = str+1;
	}
	return 1;
}
