#include "monitor.h"

extern "C"

int kernel_main(struct multiboot *mboot_ptr){
  monitor_initialize();
  monitor_clear();
  monitor_write("SmithOS v0.0.1 by AgenteSmith");
return 0xDEADBEEF;
}
























































