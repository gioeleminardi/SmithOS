// monitor.cpp

#include "monitor.h"

uint16_t *video_memory = (uint16_t*)0xB8000;
uint8_t cursor_x;
uint8_t cursor_y;
uint8_t terminal_color;
  

uint8_t make_color(enum vga_color fg, enum vga_color bg){
  return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color){
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << 8;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
  const size_t index = y * VGA_WIDTH + x;
  video_memory[index] = make_vgaentry(c, color);
}

/*static void terminal_setcolor(uint8_t color){
  terminal_color = color;
}*/

void monitor_initialize(){
  cursor_x = 0; //column
  cursor_y = 0; //row
  terminal_color = make_color(COLOR_WHITE, COLOR_BLACK);  
}

static void move_cursor(){
  uint16_t cursorLocation = cursor_y * VGA_WIDTH + cursor_x;
  outb(0x3D4, 14);
  outb(0x3D5, cursorLocation >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, cursorLocation);
}

static void scroll(){
  uint16_t blank = make_vgaentry(' ', terminal_color);
  if(cursor_y > VGA_HEIGHT){
    size_t i;
    for(i = 0*VGA_WIDTH; i < VGA_HEIGHT*VGA_WIDTH; i++){
      video_memory[i] = video_memory[i+VGA_WIDTH];
    }
    for(i = VGA_HEIGHT*VGA_WIDTH; i < (VGA_HEIGHT+1)*VGA_WIDTH; i++){
      video_memory[i] = blank;
    }
    cursor_y = VGA_HEIGHT;
  }
}

void monitor_put(char c){
  if(c == 0x08 && cursor_x){
    cursor_x--;
  }else if(c == 0x09){
    cursor_x = (cursor_x+8) & ~(8-1);
  }else if(c == '\r'){
    cursor_x = 0;
  }else if(c == '\n'){
    cursor_x = 0;
    cursor_y++;
  }else if(c >= ' '){
    terminal_putentryat(c, terminal_color, cursor_x, cursor_y);
    cursor_x++;
  }
  if(cursor_x >= VGA_WIDTH){
    cursor_x = 0;
    cursor_y++;
  }
  scroll();
  move_cursor();
}

void monitor_clear(){
  size_t x, y;
  for(y = 0; y < VGA_HEIGHT; y++){
    for(x = 0; x < VGA_WIDTH; x++){
      terminal_putentryat(' ', terminal_color, x, y);
    }
  }
  monitor_initialize();
  move_cursor();
}

void monitor_write(const char *c){
  size_t i = 0;
  while(c[i]){
    monitor_put(c[i++]);
  }
}

void monitor_write_hex(uint32_t n){
  int32_t tmp;
  monitor_write("0x");
  char noZeroes = 1;
  int i;
  for(i = 28; i > 0; i -= 4){
    tmp = (n >> i) & 0xF;
    if(tmp == 0 && noZeroes !=0){
      continue;
    }
    if(tmp >= 0xA){
      noZeroes = 0;
      monitor_put(tmp-0xA+'a');
    }else{
      noZeroes = 0;
      monitor_put(tmp+'0');
    }
  }
  tmp = n & 0xF;
  if(tmp >= 0xA){
    monitor_put(tmp-0xA+'a');
  }else{
    monitor_put(tmp+'0');
  }
}

void monitor_write_dec(uint32_t n){
  if(n==0){
    monitor_put('0');
    return;
  }
  int32_t acc = n;
  char c[32];
  int i = 0;
  while(acc > 0){
    c[i] = '0' + acc%10;
    acc /= 10;
    i++;
  }
  c[i]=0;
  char c2[32];
  c2[i--] = 0;
  int j = 0;
  while(i >= 0){
    c2[i--] = c[j++];
  }
  monitor_write(c2);
}
