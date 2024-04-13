static void _6502_anal_esil_get_addr_pattern1(RAnalOp *op, const ut8* data, char* addrbuf, int addrsize) {
	switch(data[0] & 0x1f) { // 0x1f = b00111111
	case 0x09: // op #$ff
		op->cycles = 2;
		snprintf (addrbuf, addrsize,"0x%02x", data[1]);
		break;
	case 0x05: // op $ff
		op->cycles = 3;
		snprintf (addrbuf, addrsize,"0x%02x", data[1]);
		break;
	case 0x15: // op $ff,x
		op->cycles = 4;
		snprintf (addrbuf, addrsize,"x,0x%02x,+", data[1]);
		break;
	case 0x0d: // op $ffff
		op->cycles = 4;
		snprintf (addrbuf, addrsize,"0x%04x", data[1] | data[2] << 8);
		break;
	case 0x1d: // op $ffff,x
		op->cycles = 4;
		snprintf (addrbuf, addrsize,"x,0x%04x,+", data[1] | data[2] << 8);
		break;
	case 0x19: // op $ffff,y
		op->cycles = 4;
		snprintf (addrbuf, addrsize,"y,0x%04x,+", data[1] | data[2] << 8);
		break;
	case 0x01: // op ($ff,x)
		op->cycles = 6;
		snprintf (addrbuf, addrsize,"x,0x%02x,+,[2]", data[1]);
		break;
	case 0x11: // op ($ff),y
		op->cycles = 5;
		snprintf (addrbuf, addrsize,"y,0x%02x,[2],+", data[1]);
		break;
	}
}
