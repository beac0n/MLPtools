

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>

//serial stuff
#include <termios.h>
#include <unistd.h>

#include <stdint.h>

#include "led_utils.h"

struct ledPanel * openConnection(char *device, unsigned int stripLen, unsigned int width){
    
    struct ledPanel * panel = malloc(sizeof(struct ledPanel));
    

    panel->fd = open(device, O_RDWR | O_NOCTTY |O_SYNC);
    
    struct termios termios_p;
    cfmakeraw(&termios_p);
    tcsetattr(panel->fd, TCSANOW, &termios_p);
    
    
    
    panel->width = width;
    panel->stripLen = stripLen;
    panel->buffer = malloc(sizeof(int)*stripLen*6);


}

void update(struct ledPanel * panel){
    
//    printf("updating...");
  //  fflush(stdout);
    //int written = write(panel->fd, panel->buffer, panel->stripLen*24);
    //char nl = '\n';
    //write(panel->fd, "Begin", 5);
    int written = write(panel->fd, (char*)panel->buffer, 7200);
    //fsync(panel->fd);

    //tcdrain(panel->fd);
    //printf("sent %d...", written);
    //fflush(stdout);

    char test[1];
    
   /* 
    int i;
    for(i = 0; i<7200;i++){
        read(panel->fd, test, 1);
        printf("at %4d should be %8d read %8d\n", i,((char*)panel->buffer)[i], test[0]);
        
        if(((char*)panel->buffer)[i] != test[0]){
            printf("difference at %d!!!\n", i);
        }
        fflush(stdout);
    }
*/
    
    //char nl = '\n';
    //write(panel->fd, &nl, 1);
    //write(panel->fd, &nl, 1);

    //char buffer[1];

    //printf("waiting...");
    //fflush(stdout);
    //while (buffer[0] != '\n'){
    //    read(panel->fd, buffer, 1);
    //}
    //printf("done!\n");
    //fflush(stdout);
    //printf("\t\t\t\t\t\t wrote %d\n", written);
    
    
    
    /*int i;
    for(i=0; i<7200;i++){
        char byte = ((char*)panel->buffer)[i];
        
        int j;
        for(j=0;j<8;j++){
            printf("%d ", (byte>>(7-j))&1);
        }
        printf("\n");
    }
    printf("\n");*/
}




void setPixel(struct ledPanel * panel, uint32_t num, int color){
    if(num >= 2400) return;
    if((num/60)%2 == 0){
        num = (num/60)*60+59-(num%60);
    }

    //((char*)panel->buffer)[num*3+0] = ((color>>16) & 0xFF);
    //((char*)panel->buffer)[num*3+1] = ((color>>8 ) & 0xFF);
    //((char*)panel->buffer)[num*3+2] = ((color    ) & 0xFF);
    
    if(color > 0xFFFFFF){
        printf("Strange color?????????\n");
    }

    //printf("%d %d %d\n", (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);


   
    uint32_t strip, offset, mask;
    uint8_t bit, *p;
    
    color = ((color<<8)&0xFF0000) | ((color>>8)&0x00FF00) | (color&0x0000FF);
    strip = num / 300;
    offset = num % 300;
    bit = (1<<strip);
    p = ((uint8_t *)panel->buffer) + offset * 24;
    for (mask = (1<<23) ; mask ; mask >>= 1) {
        if (color & mask) {
            *p++ |= bit;
        } else {
            *p++ &= ~bit;
        }
    }
}

/*
void setPixel(struct ledPanel * panel, uint32_t num, int color){
    if(num >= 2400) return;
    if((num/60)%2 == 0){
        num = (num/60)*60+59-(num%60);
    }
   
   
    uint32_t strip, offset, mask;
    uint8_t bit, *p;
    
    color = ((color<<8)&0xFF0000) | ((color>>8)&0x00FF00) | (color&0x0000FF);
    strip = num / panel->stripLen;
    offset = num % panel->stripLen;
    bit = (1<<strip);
    p = ((uint8_t *)panel->buffer) + offset * 24;
    for (mask = (1<<23) ; mask ; mask >>= 1) {
        if (color & mask) {
            *p++ |= bit;
        } else {
            *p++ &= ~bit;
        }
    }
}
*/



