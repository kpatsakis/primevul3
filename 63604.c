static void setup_report_key(struct packet_command *cgc, unsigned agid, unsigned type)
{
	cgc->cmd[0] = GPCMD_REPORT_KEY;
	cgc->cmd[10] = type | (agid << 6);
	switch (type) {
		case 0: case 8: case 5: {
			cgc->buflen = 8;
			break;
		}
		case 1: {
			cgc->buflen = 16;
			break;
		}
		case 2: case 4: {
			cgc->buflen = 12;
			break;
		}
	}
	cgc->cmd[9] = cgc->buflen;
	cgc->data_direction = CGC_DATA_READ;
}
