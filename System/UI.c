#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include <string.h>

typedef struct {
    char line1[16];
    char line2[16];
    char line3[16];
    char line4[16];
    uint8_t exist_title;
} UI_typedef;

UI_typedef root;


void UI_Init(void)
{
    OLED_Init();
    /* 主菜单 */
    strcpy(root.line1, "Start");
    strcpy(root.line1, "PID");
    strcpy(root.line1, "");
    strcpy(root.line1, "");
}
