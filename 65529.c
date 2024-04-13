static ssize_t uvesafb_show_nocrtc(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *info = platform_get_drvdata(to_platform_device(dev));
	struct uvesafb_par *par = info->par;

	return snprintf(buf, PAGE_SIZE, "%d\n", par->nocrtc);
}
