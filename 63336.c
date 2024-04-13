static void print_usage(void)
{
	printf("mosquitto version %s\n\n", VERSION);
	printf("mosquitto is an MQTT v3.1.1 broker.\n\n");
	printf("Usage: mosquitto [-c config_file] [-d] [-h] [-p port]\n\n");
	printf(" -c : specify the broker config file.\n");
	printf(" -d : put the broker into the background after starting.\n");
	printf(" -h : display this help.\n");
	printf(" -p : start the broker listening on the specified port.\n");
	printf("      Not recommended in conjunction with the -c option.\n");
	printf(" -v : verbose mode - enable all logging types. This overrides\n");
	printf("      any logging options given in the config file.\n");
	printf("\nSee http://mosquitto.org/ for more information.\n\n");
}
