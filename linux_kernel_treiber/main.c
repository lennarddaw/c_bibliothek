// ich versuche mich hier an einem kernel module, also keinen Linux Kernel
// der code hier kommt also in den laufenden kernel (bzw. er kommt da auch wieder raus, deshalb die cleanup function)

#if defined(__KERNEL__) // hier fragen ob ich kernel oder user space läuft automatisch 
#include <linux/kernel.h>
#include <linux/module.h>
#else
#include <stdio.h> // die gesamte else konstruktion brauche ich nur, damit ich es auch im User space compilen kann
#define printk printf 
#define KERN_INFO ""
#endif

// die ganze function gibt denke ich an, dass das Laden
// auch erfolgreich war bei was anderem als return 0 wird dann halt abgebrochen
int init_module(void) {
    printk(KERN_INFO "Hello, Kernel\n");
    return 0;
}

// das was ich in init function reserviert habe gebe ich hier einfach
// wieder frei 
// Die Frage stellt sich warum dann aber nur ein print Statement?
// Anscheinend kennt der kernel exakt diese beiden methoden mit namen
// die print statements sind daher nur für den user und die functions würden auch so laufen
void cleanup_module(void) { // anscheined muss ich hier typ void explizit übergeben, da () alles sein kann
    printk(KERN_INFO "Bye, Kernel");
}

// Bisher habe ich keine Plan ob das so läuft (denke schon, weil viel ists ja nichts)
// bin hier auf windows und wenn ich per MinGW compile wird das zu ner windows .exe
// aber der kernel ist eine .ko versuche es daher gerade in ubuntu bzw. mit der Linux VM


