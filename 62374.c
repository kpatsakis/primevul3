sendNotifications(int notif_type, const struct device * dev, const struct service * serv)
{
	struct reqelem * req;
	unsigned int m;
	unsigned char rbuf[RESPONSE_BUFFER_SIZE];
	unsigned char * rp;

	for(req = reqlisthead.lh_first; req; req = req->entries.le_next) {
		if(!req->is_notify) continue;
		rbuf[0] = '\xff'; /* special code for notifications */
		rbuf[1] = (unsigned char)notif_type;
		rbuf[2] = 0;
		rp = rbuf + 3;
		if(dev) {
			/* response :
			 * 1 - Location
			 * 2 - NT (device/service type)
			 * 3 - usn */
			m = dev->headers[HEADER_LOCATION].l;
			CODELENGTH(m, rp);
			memcpy(rp, dev->headers[HEADER_LOCATION].p, dev->headers[HEADER_LOCATION].l);
			rp += dev->headers[HEADER_LOCATION].l;
			m = dev->headers[HEADER_NT].l;
			CODELENGTH(m, rp);
			memcpy(rp, dev->headers[HEADER_NT].p, dev->headers[HEADER_NT].l);
			rp += dev->headers[HEADER_NT].l;
			m = dev->headers[HEADER_USN].l;
			CODELENGTH(m, rp);
			memcpy(rp, dev->headers[HEADER_USN].p, dev->headers[HEADER_USN].l);
			rp += dev->headers[HEADER_USN].l;
			rbuf[2]++;
		}
		if(serv) {
			/* response :
			 * 1 - Location
			 * 2 - NT (device/service type)
			 * 3 - usn */
			m = strlen(serv->location);
			CODELENGTH(m, rp);
			memcpy(rp, serv->location, m);
			rp += m;
			m = strlen(serv->st);
			CODELENGTH(m, rp);
			memcpy(rp, serv->st, m);
			rp += m;
			m = strlen(serv->usn);
			CODELENGTH(m, rp);
			memcpy(rp, serv->usn, m);
			rp += m;
			rbuf[2]++;
		}
		if(rbuf[2] > 0) {
			if(write_or_buffer(req, rbuf, rp - rbuf) < 0) {
				syslog(LOG_ERR, "(s=%d) write: %m", req->socket);
				/*goto error;*/
			}
		}
	}
}
