#include "pic.h"
#include "port_io.h"

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */

#define PIC1_COMMAND_PORT PIC1
#define PIC1_DATA_PORT    (PIC1 + 1)
#define PIC2_COMMAND_PORT PIC2
#define PIC2_DATA_PORT    (PIC2 + 1)

void pic_disable(void)
{
    outb(PIC1_DATA_PORT, 0xFF);
    outb(PIC2_DATA_PORT, 0xFF);
}

#define ICW1_ICW4      0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE    0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL     0x08 /* Level triggered (edge) mode */
#define ICW1_INIT      0x10 /* Initialization - required! */
 
#define ICW4_8086       0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM       0x10 /* Special fully nested (not) */

void pic_remap(uint8_t offset1, uint8_t offset2)
{
    // Preserve existing masks.
	uint8_t a1 = inb(PIC1_DATA_PORT);
	uint8_t a2 = inb(PIC2_DATA_PORT);

    // starts the initialization sequence (in cascade mode)
	outb(PIC1_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	io_wait();

    // ICW2: Master PIC vector offset
	outb(PIC1_DATA_PORT, offset1);
	io_wait();
    // ICW2: Slave PIC vector offset
	outb(PIC2_DATA_PORT, offset2);
	io_wait();
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC1_DATA_PORT, 4);
	io_wait();
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(PIC2_DATA_PORT, 2);
	io_wait();

	outb(PIC1_DATA_PORT, ICW4_8086);
	io_wait();
	outb(PIC2_DATA_PORT, ICW4_8086);
	io_wait();

    // Restore saved masks.
	outb(PIC1_DATA_PORT, a1);
	outb(PIC2_DATA_PORT, a2);
}

#define PIC_EOI 0x20

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(PIC2_COMMAND_PORT, PIC_EOI);
    }

    outb(PIC1_COMMAND_PORT, PIC_EOI);
}

static inline uint16_t pic_get_port(uint8_t irq)
{
    return irq < 8 ? PIC1_DATA_PORT : PIC2_DATA_PORT;
}

static inline uint8_t pic_get_mask_bit(uint8_t irq)
{
    return 1 << (irq & 0b0111);
}

void pic_set_mask(uint8_t irq)
{
	uint16_t port = pic_get_port(irq);
	uint8_t value = inb(port);

	outb(port, value | pic_get_mask_bit(irq));
}

void pic_clear_mask(uint8_t irq)
{
	uint16_t port = pic_get_port(irq);
	uint8_t value = inb(port);

	outb(port, value & ~pic_get_mask_bit(irq));
}

static inline uint16_t pic_get_irq_register(uint8_t ocw3)
{
	outb(PIC1_COMMAND_PORT, ocw3);
	outb(PIC2_COMMAND_PORT, ocw3);

	return (inb(PIC2_COMMAND_PORT) << 8) | inb(PIC1_COMMAND_PORT);
}

#define PIC_READ_IRR 0x0A
#define PIC_READ_ISR 0x0B

uint16_t pic_get_irr(void)
{
	return pic_get_irq_register(PIC_READ_IRR);
}

uint16_t pic_get_isr(void)
{
	return pic_get_irq_register(PIC_READ_ISR);
}
