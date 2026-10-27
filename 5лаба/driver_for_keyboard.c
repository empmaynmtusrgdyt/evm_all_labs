#include <linux/module.h> // делает код загружаемым модулем ядра
#include <linux/kernel.h> // тут живут базовые ф-ции ядра
#include <linux/interrupt.h> // заголовок для работы с прерываниями


static int irq_number = 24; // номер прерывания
static int irq_count = 0; // переменная-счетчик

static irqreturn_t interrupt_handler(int irq, void *dev_id){ // ф-ция обработки
	irq_count++; // увеличение глобального счетчика
	if(irq_count % 10 == 0) printk(KERN_INFO "Lab5: count = %d\n", irq_count); // пишем в лог ядра
	return IRQ_HANDLED; // безопасное заверешение прерывания
}

static int __init driver_init(void){ // ф-ция инициализации
	int result;
	result = request_irq(irq_number, interrupt_handler, IRQF_SHARED, "lab5", &irq_count); // просим ядро зарегать нашу функцию-обработчик
	// IRQF_SHARED означает что мы делим прерывание с работой системной клавиатуры
	if(result != 0){
		printk(KERN_ERR "Lab5: failed to request IRQ %d\n", irq_number);
		return result;
	}
	printk(KERN_INFO "Lab5: driver loaded successfully!\n");
	return 0;
}

static void __exit driver_exit(void){ // ф-ция выгрузки
	free_irq(irq_number, &irq_count); // освобождаем линию прерывания (24)
	printk(KERN_INFO "Lab 5: driver unloaded. Total count = %d\n", irq_count);
}

module_init(driver_init);
module_exit(driver_exit);
MODULE_LICENSE("GPL");
