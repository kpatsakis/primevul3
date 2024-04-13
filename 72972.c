open_interface(const char *device, netdissect_options *ndo, char *ebuf)
{
	pcap_t *pc;
#ifdef HAVE_PCAP_CREATE
	int status;
	char *cp;
#endif

#ifdef HAVE_PCAP_CREATE
	pc = pcap_create(device, ebuf);
	if (pc == NULL) {
		/*
		 * If this failed with "No such device", that means
		 * the interface doesn't exist; return NULL, so that
		 * the caller can see whether the device name is
		 * actually an interface index.
		 */
		if (strstr(ebuf, "No such device") != NULL)
			return (NULL);
		error("%s", ebuf);
	}
#ifdef HAVE_PCAP_SET_TSTAMP_TYPE
	if (Jflag)
		show_tstamp_types_and_exit(pc, device);
#endif
#ifdef HAVE_PCAP_SET_TSTAMP_PRECISION
	status = pcap_set_tstamp_precision(pc, ndo->ndo_tstamp_precision);
	if (status != 0)
		error("%s: Can't set %ssecond time stamp precision: %s",
			device,
			tstamp_precision_to_string(ndo->ndo_tstamp_precision),
			pcap_statustostr(status));
#endif

#ifdef HAVE_PCAP_SET_IMMEDIATE_MODE
	if (immediate_mode) {
		status = pcap_set_immediate_mode(pc, 1);
		if (status != 0)
			error("%s: Can't set immediate mode: %s",
			device,
			pcap_statustostr(status));
	}
#endif
	/*
	 * Is this an interface that supports monitor mode?
	 */
	if (pcap_can_set_rfmon(pc) == 1)
		supports_monitor_mode = 1;
	else
		supports_monitor_mode = 0;
	status = pcap_set_snaplen(pc, ndo->ndo_snaplen);
	if (status != 0)
		error("%s: Can't set snapshot length: %s",
		    device, pcap_statustostr(status));
	status = pcap_set_promisc(pc, !pflag);
	if (status != 0)
		error("%s: Can't set promiscuous mode: %s",
		    device, pcap_statustostr(status));
	if (Iflag) {
		status = pcap_set_rfmon(pc, 1);
		if (status != 0)
			error("%s: Can't set monitor mode: %s",
			    device, pcap_statustostr(status));
	}
	status = pcap_set_timeout(pc, 1000);
	if (status != 0)
		error("%s: pcap_set_timeout failed: %s",
		    device, pcap_statustostr(status));
	if (Bflag != 0) {
		status = pcap_set_buffer_size(pc, Bflag);
		if (status != 0)
			error("%s: Can't set buffer size: %s",
			    device, pcap_statustostr(status));
	}
#ifdef HAVE_PCAP_SET_TSTAMP_TYPE
	if (jflag != -1) {
		status = pcap_set_tstamp_type(pc, jflag);
		if (status < 0)
			error("%s: Can't set time stamp type: %s",
		              device, pcap_statustostr(status));
		else if (status > 0)
			warning("When trying to set timestamp type '%s' on %s: %s",
				pcap_tstamp_type_val_to_name(jflag), device,
				pcap_statustostr(status));
	}
#endif
	status = pcap_activate(pc);
	if (status < 0) {
		/*
		 * pcap_activate() failed.
		 */
		cp = pcap_geterr(pc);
		if (status == PCAP_ERROR)
			error("%s", cp);
		else if (status == PCAP_ERROR_NO_SUCH_DEVICE) {
			/*
			 * Return an error for our caller to handle.
			 */
			snprintf(ebuf, PCAP_ERRBUF_SIZE, "%s: %s\n(%s)",
			    device, pcap_statustostr(status), cp);
			pcap_close(pc);
			return (NULL);
		} else if (status == PCAP_ERROR_PERM_DENIED && *cp != '\0')
			error("%s: %s\n(%s)", device,
			    pcap_statustostr(status), cp);
		else
			error("%s: %s", device,
			    pcap_statustostr(status));
	} else if (status > 0) {
		/*
		 * pcap_activate() succeeded, but it's warning us
		 * of a problem it had.
		 */
		cp = pcap_geterr(pc);
		if (status == PCAP_WARNING)
			warning("%s", cp);
		else if (status == PCAP_WARNING_PROMISC_NOTSUP &&
		         *cp != '\0')
			warning("%s: %s\n(%s)", device,
			    pcap_statustostr(status), cp);
		else
			warning("%s: %s", device,
			    pcap_statustostr(status));
	}
#ifdef HAVE_PCAP_SETDIRECTION
	if (Qflag != -1) {
		status = pcap_setdirection(pc, Qflag);
		if (status != 0)
			error("%s: pcap_setdirection() failed: %s",
			      device,  pcap_geterr(pc));
		}
#endif /* HAVE_PCAP_SETDIRECTION */
#else /* HAVE_PCAP_CREATE */
	*ebuf = '\0';
	pc = pcap_open_live(device, ndo->ndo_snaplen, !pflag, 1000, ebuf);
	if (pc == NULL) {
		/*
		 * If this failed with "No such device", that means
		 * the interface doesn't exist; return NULL, so that
		 * the caller can see whether the device name is
		 * actually an interface index.
		 */
		if (strstr(ebuf, "No such device") != NULL)
			return (NULL);
		error("%s", ebuf);
	}
	if (*ebuf)
		warning("%s", ebuf);
#endif /* HAVE_PCAP_CREATE */

	return (pc);
}
