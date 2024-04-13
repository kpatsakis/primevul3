static void setup_send_key(struct packet_command *cgc, unsigned agid, unsigned type)
{
	cgc->cmd[0] = GPCMD_SEND_KEY;
	cgc->cmd[10] = type | (agid << 6);
	switch (type) {
		case 1: {
			cgc->buflen = 16;
			break;
		}
		case 3: {
			cgc->buflen = 12;
			break;
		}
		case 6: {
			cgc->buflen = 8;
			break;
		}
	}
	cgc->cmd[9] = cgc->buflen;
	cgc->data_direction = CGC_DATA_WRITE;
}
