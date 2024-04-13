static char* _resource_type_str(int type) {
	switch (type) {
	case 1: return "CURSOR";
	case 2: return "BITMAP";
	case 3: return "ICON";
	case 4: return "MENU";
	case 5: return "DIALOG";
	case 6: return "STRING";
	case 7: return "FONTDIR";
	case 8: return "FONT";
	case 9: return "ACCELERATOR";
	case 10: return "RCDATA";
	case 11: return "MESSAGETABLE";
	case 12: return "GROUP_CURSOR";
	case 14: return "GROUP_ICON";
	case 16: return "VERSION";
	case 17: return "DLGINCLUDE";
	case 19: return "PLUGPLAY";
	case 20: return "VXD";
	case 21: return "ANICURSOR";
	case 22: return "ANIICON";
	case 23: return "HTML";
	case 24: return "MANIFEST";
	default: return "UNKNOWN";
	}
}
