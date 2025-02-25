#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define bton1 2
#define bton2 4
#define bton3 7
#define bton4 9

#define LED_VR 11
#define LED_VE 12
#define LED_AZ 13

#define DO  262
#define RE  294
#define MI  330
#define FA  349
#define SO  392

#define BUZZER_PIN 0 
#define DUTY_CYCLE 32767
#define LED_ON 0  
#define LED_OFF 1  

void nota(uint slice_num, int freq) {
    if (freq > 0) {
        uint32_t clock = 125000000;
        uint32_t divider = 64;
        uint32_t wrap = (clock / divider) / freq;

        pwm_set_clkdiv(slice_num, divider);
        pwm_set_wrap(slice_num, wrap);
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), wrap / 2);
        pwm_set_enabled(slice_num, true);
    }
}

void LED(int r, int g, int b) {
    gpio_put(LED_VR, r);
    gpio_put(LED_VE, g);
    gpio_put(LED_AZ, b);
}

int main() {
    stdio_init_all();
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);


 int teclas[] = {bton1, bton2, bton3, bton4};
    for (int i = 0; i < 4; i++) {
        gpio_init(teclas[i]);
        gpio_set_dir(teclas[i], GPIO_IN);
        gpio_pull_up(teclas[i]);
    }

    gpio_init(LED_VR);
    gpio_set_dir(LED_VR, GPIO_OUT);
    gpio_init(LED_VE);
    gpio_set_dir(LED_VE, GPIO_OUT);
    gpio_init(LED_AZ);
    gpio_set_dir(LED_AZ, GPIO_OUT);
     
    while (1) {
        if (!gpio_get(bton1)) {
            nota(slice_num, DO);
            LED(LED_ON, LED_OFF, LED_OFF);  // Acende Vermelho
        } else if (!gpio_get(bton2)) {
            nota(slice_num, RE);
            LED(LED_OFF, LED_ON, LED_OFF);  // Acende Verde
        } else if (!gpio_get(bton3)) {
            nota(slice_num, MI);
            LED(LED_OFF, LED_OFF, LED_ON);  // Acende Azul
        } else if (!gpio_get(bton4)) {
            nota(slice_num, FA);
            LED(LED_ON, LED_ON, LED_OFF);  // Acende Amarelo
        } else {
            pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), 0);
            LED(LED_OFF, LED_OFF, LED_OFF);
        }
        sleep_ms(100);
    }

    return 0;
}