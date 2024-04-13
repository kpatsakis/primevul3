static void stdpr(struct printparam* pp, char *buf, int inbuf){
	if((pp->inbuf + inbuf > 1024) || !buf) {
		socksend(pp->cp->clisock, (unsigned char *)pp->buf, pp->inbuf, conf.timeouts[STRING_S]);
		pp->inbuf = 0;
		if(!buf) return;
	}
	if(inbuf >= 1000){
		socksend(pp->cp->clisock, (unsigned char *)buf, inbuf, conf.timeouts[STRING_S]);		
	}
	else {
		memcpy(pp->buf + pp->inbuf, buf, inbuf);
		pp->inbuf += inbuf;
	}
}
