# Also warum dieses repo hier? 

Ich habe mir gedacht, dass es sinnvoll ist mit C anzufangen um näher an der Hardware zu arbeiten. Derzeit habe ich noch nicht viele Vorkenntnisse. Syntax und die basic funktionen hab ich mir auf YouTube bei [MagicalBat](https://www.youtube.com/watch?v=4-0GTazM5rY) angeschaut 

## Was ist das?

Ein Kernel-Modul ist **kein** normales Programm mit `main()`. Es ist Code, der
zur Laufzeit in den laufenden Linux-Kernel geladen wird und dort lebt. Statt
eines Startpunkts hat es zwei Ereignis-Funktionen:

- **`init_module()`** — wird beim Laden (`insmod`) aufgerufen
- **`cleanup_module()`** — wird beim Entladen (`rmmod`) aufgerufen

Ausgaben laufen über `printk` (statt `printf`) und landen im Kernel-Log, das
man mit `dmesg` ausliest.

## Voraussetzungen

- Ein **echtes Linux** (z. B. Ubuntu in einer VM oder als Dual-Boot)
- Build-Tools und die Kernel-Header der laufenden Version:

```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```

> ⚠️ **WSL2-Hinweis:** WSL2 nutzt einen eigenen Microsoft-Kernel und liefert
> standardmäßig **keine** Modul-Header mit — `/lib/modules/$(uname -r)/build`
> ist leer. Dort lässt sich ein Modul ohne vorherigen Eigen-Bau des
> WSL2-Kernels weder kompilieren noch laden. Für den Einstieg ist eine echte
> Ubuntu-VM deutlich angenehmer.

## Projektstruktur

```
linux_kernel_treiber/
├── main.c       # der Modul-Quellcode
├── Makefile     # Kbuild-Makefile
└── README.md
```

## Code (Kern)

```c
#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void) {
    printk(KERN_INFO "Hello, Kernel\n");   // kein Komma nach KERN_INFO!
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Bye, Kernel\n");
}

MODULE_LICENSE("GPL");
```

## Makefile

```makefile
obj-m += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

## Bauen

```bash
make
```

Ergebnis ist eine `main.ko` (kernel object) — das ist das fertige Modul.

## Laden, testen, entladen

```bash
sudo insmod main.ko      # Modul laden
dmesg | tail             # -> "Hello, Kernel"

lsmod | grep main        # prüfen, ob es geladen ist

sudo rmmod main          # Modul entladen
dmesg | tail             # -> "Bye, Kernel"
```

## Stolpersteine (aus der Praxis)

- **`printk(KERN_INFO, "...")`** mit Komma ist falsch. `KERN_INFO` ist ein
  String-Literal, das direkt mit dem Text verschmolzen wird:
  `printk(KERN_INFO "...")` — **ohne** Komma.
- **`MODULE_LICENSE("GPL")`** nicht vergessen, sonst "tainted kernel"-Warnung.
- **`(void)`** statt leerer Klammern bei den Funktionen (kernel-üblich).
- Ein Kernel-Modul lässt sich **nicht** mit `gcc datei.c -o datei.exe` bauen
  und **nicht** unter Windows laden — es braucht das Kbuild-System und einen
  laufenden Linux-Kernel.