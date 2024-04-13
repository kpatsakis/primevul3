static void stdcbf(void *cb, char *buf, int inbuf){
	int delay = 0;
	int i;

	for(i = 0; i < inbuf; i++){
		switch(buf[i]){
			case '&':
				if(delay){
					stdpr((struct printparam*)cb, buf+i-delay, delay);
					delay = 0;
				}
				stdpr((struct printparam*)cb, "&amp;", 5);
				break;
			case '<':
				if(delay){
					stdpr((struct printparam*)cb, buf+i-delay, delay);
					delay = 0;
				}
				stdpr((struct printparam*)cb, "&lt;", 4);
				break;
			case '>':
				if(delay){
					stdpr((struct printparam*)cb, buf+i-delay, delay);
					delay = 0;
				}
				stdpr((struct printparam*)cb, "&gt;", 4);
				break;
			default:
				delay++;
				break;
		}
	}
	if(delay){
		stdpr((struct printparam*)cb, buf+i-delay, delay);
	}
}
