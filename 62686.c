void v4l_printk_ioctl(unsigned int cmd)
{
	char *dir, *type;

	switch (_IOC_TYPE(cmd)) {
	case 'd':
		type = "v4l2_int";
		break;
	case 'V':
		if (_IOC_NR(cmd) >= V4L2_IOCTLS) {
			type = "v4l2";
			break;
		}
		printk("%s", v4l2_ioctls[_IOC_NR(cmd)]);
		return;
	default:
		type = "unknown";
	}

	switch (_IOC_DIR(cmd)) {
	case _IOC_NONE:              dir = "--"; break;
	case _IOC_READ:              dir = "r-"; break;
	case _IOC_WRITE:             dir = "-w"; break;
	case _IOC_READ | _IOC_WRITE: dir = "rw"; break;
	default:                     dir = "*ERR*"; break;
	}
	printk("%s ioctl '%c', dir=%s, #%d (0x%08x)",
		type, _IOC_TYPE(cmd), dir, _IOC_NR(cmd), cmd);
 }
