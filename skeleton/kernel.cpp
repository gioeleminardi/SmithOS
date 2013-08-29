#include "monitor.h"

extern "C"

int kernel_main(/*struct multiboot *mboot_ptr*/){
  monitor_initialize();
  monitor_clear();
  monitor_write("SmithOS v0.0.1 by AgenteSmith ");
  uint32_t i;
  for(i = 0; i<10000; i++){
    monitor_write_dec(i);
    monitor_write("\n");
  }
return 0xDEADBEEF;
}
























































