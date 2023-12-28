#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Yuliia Tyshkevych");
// Вказує автора модуля

MODULE_DESCRIPTION("Simple chrono counter for my lab");
// Надає опис модуля

MODULE_LICENSE("Dual BSD/GPL");
// Вказує ліцензію для модуля

static uint chrono_counter = 1;
// Оголошення та ініціалізація змінної chrono_counter

module_param(chrono_counter, uint, 0444);
// Вказує, що chrono_counter є параметром модуля, доступним для зчитування

MODULE_PARM_DESC(chrono_counter, "counter");
// Надання опису параметра chrono_counter

struct my_struct {
    struct list_head tlist;
    ktime_t time;
};
// Оголошення структури my_struct, яка містить список та час

static LIST_HEAD(my_list_head);
// Ініціалізація голови списку my_list_head

static int __init hello_init(void)
{
    if (chrono_counter == 0) {
        printk(KERN_WARNING "Success! The parameter's 0\n");
        // Вивести повідомлення з рівнем KERN_WARNING, якщо chrono_counter == 0
    } else if (chrono_counter >= 5 && chrono_counter <= 10) {
        printk(KERN_WARNING "Success! The parameter's between 5 and 10\n");
        // Вивести повідомлення з рівнем KERN_WARNING, якщо chrono_counter >= 5 та <= 10
    } else if (chrono_counter > 10) {
        printk(KERN_WARNING "Fail! The parameter's > 10)\n");
        // Вивести повідомлення з рівнем KERN_WARNING, якщо chrono_counter > 10
        return -EINVAL;
        // Повернути помилку -EINVAL
    }

    int i;
    // Оголошення змінної i

    for (i = 0; i < chrono_counter; i++) {
        struct my_struct *ms = NULL;
        // Оголошення вказівника на структуру my_struct

        ms = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
        // Виділення пам'яті для структури my_struct з використанням kmalloc

        ms->time = ktime_get();
        // Запис поточного часу до полу my_struct

        list_add_tail(&ms->tlist, &my_list_head);
        // Додавання елементу до списку ззаду

        printk(KERN_INFO "Hello, world!\n");
        // Виведення повідомлення з рівнем KERN_INFO
    }

    return 0;
    // Повернення 0, що означає успішне завершення функції ініціалізації
}

static void __exit hello_exit(void)
{
    int count = 0;
    // Оголошення змінної count

    struct my_struct *t, *tmp;
    // Оголошення вказівників на структуру my_struct

    list_for_each_entry_safe(t, tmp, &my_list_head, tlist) {
        pr_info("i: %d; ktime = %lld\n", count++, ktime_to_ns(t->time));
        // Виведення інформації про елемент списку та час у наносекундах
        list_del(&t->tlist);
        // Видалення елементу зі списку
        kfree(t);
        // Звільнення виділеної пам'яті
    }
    printk(KERN_INFO "Success! Module unloaded\n");
    // Виведення повідомлення з рівнем KERN_INFO
}

module_init(hello_init);
// Вказання функції ініціалізації модуля для ядра Linux

module_exit(hello_exit);
// Вказання функції завершення модуля для ядра Linux

