static void tun_put(struct tun_struct *tun)
{
	dev_put(tun->dev);
}
