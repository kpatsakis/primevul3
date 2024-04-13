static void amd_gpio_irq_enable(struct irq_data *d)
{
	u32 pin_reg;
	unsigned long flags;
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct amd_gpio *gpio_dev = gpiochip_get_data(gc);

	spin_lock_irqsave(&gpio_dev->lock, flags);
	pin_reg = readl(gpio_dev->base + (d->hwirq)*4);
	/*
		Suppose BIOS or Bootloader sets specific debounce for the
		GPIO. if not, set debounce to be  2.75ms.
	*/
	if ((pin_reg & DB_TMR_OUT_MASK) == 0) {
		pin_reg |= 0xf;
		pin_reg |= BIT(DB_TMR_OUT_UNIT_OFF);
		pin_reg &= ~BIT(DB_TMR_LARGE_OFF);
	}
	pin_reg |= BIT(INTERRUPT_ENABLE_OFF);
	pin_reg |= BIT(INTERRUPT_MASK_OFF);
	writel(pin_reg, gpio_dev->base + (d->hwirq)*4);
	spin_unlock_irqrestore(&gpio_dev->lock, flags);
}
