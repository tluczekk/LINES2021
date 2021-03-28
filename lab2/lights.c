#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<gpiod.h>
// Signal related code from OPS course https://sop.mini.pw.edu.pl/en/node/118
#define ERR(source) (fprintf(stderr, "%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0, SIGKILL),\
                     exit(EXIT_FAILURE))

volatile sig_atomic_t last_signal = 0;

void sig_handler(int sig){
    printf("[%d] received signal %d\n", getpid(), sig);
    last_signal = sig;
}

void flashLEDs(struct gpiod_chip *chip, struct gpiod_line_bulk leds, bool isLongFlash){
    int leds_on[8] = {1,1,1,1,1,1,1,1};
    int leds_off[8] = {0,0,0,0,0,0,0,0};
    if(gpiod_line_set_value_bulk(&leds,leds_on)<0){
        gpiod_chip_close(chip);
        ERR("Setting LEDs error");
    }

    if(isLongFlash){
        sleep(2);
    }else{
        sleep(1);
    }

    if(gpiod_line_set_value_bulk(&leds,leds_off)<0){
        gpiod_chip_close(chip);
        ERR("Setting LEDs error");
    }
}

int main(int argc, char **argv){
    srand(time(NULL));
    if(signal(SIGINT, sig_handler) == SIG_ERR) ERR("Can't intercept SIGINT");

    // OPEN GPIO
    struct gpiod_chip *chip;
    if((chip = gpiod_chip_open("/dev/gpiochip1")) == NULL){
        gpiod_chip_close(chip);
        ERR("Chip not opened");
    }

    // SWITCHES
    struct gpiod_line_bulk switches;
    int switch_num[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
    if(gpiod_chip_get_lines(chip, switch_num, 12, switches)< 0){
        gpiod_chip_close(chip);
        ERR("Switch error");
    }
    if(gpiod_line_request_both_edges_events(&switches, "switch")<0){
        gpiod_chip_close(chip);
        ERR("Switch error");
    }

    // BUTTONS
    struct gpiod_line_bulk buttons;
    int button_num[12] = {12,13,14,15,16,17,18,19,20,21,22,23};
    if(gpiod_chip_get_lines(chip, button_num,12,&buttons)<0){
        gpiod_chip_close(chip);
        ERR("Button error");
    }
    if(gpiod_line_request_bulk_falling_edge_events(&buttons, "button")<0){
        gpiod_chip_close(chip);
        ERR("Button error");
    }

    // LED
    struct gpiod_line_bulk leds;
    int led_num[8] = {24,25,26,27,28,29,30,31};
    if(gpiod_chip_get_lines(chip, led_num, 8, leds) < 0){
        gpiod_chip_close(chip);
        ERR("LED error");
    }
    if(gpiod_line_request_bulk_output(&leds, "LED", 0)<0){
        gpiod_chip_close(chip);
        ERR("LED error");
    }
    
    struct gpiod_line_event last_event;
    struct gpiod_line_bulk event_bulk;
    int event_line;
    int switch_value;
    int waited = 0;
    struct timespec shortStop = {1,0};
    struct timespec longStop = {2,0};
    int numToShow;
    puts("MORSE VISUALIZER");
    puts("LED's will show the number of pushed button");
    puts("By displaying it in the Morse Code");

    while(1){
        // Wait for user to press a button
        do{
            waited = gpiod_line_event_wait_bulk(&buttons, &longStop, &event_bulk);
            if(waited < 0 || last_signal == SIGINT)
                break;
        }while(waited<=0);
        // bouncing handling
        do{
            if(waited<0||last_signal==SIGINT)
                break;
            if(gpiod_line_event_read(event_bulk.lines[0], &last_event)<0){
                gpiod_chip_close(chip);
                ERR("GPIO event read error");
            }
            waited = gpiod_line_event_read(event_bulk.lines[0], &longStop);
        }while(waited==1);
        
        event_line = gpiod_line_offset(event_bulk.lines[0]);
        numToShow = event_line -12;
        if(numToShow < 0 || numToShow >11) ERR("Failed to get number");
        // depending on button clicked, leds will send a morse code equivalent to the user
        switch(numToShow){
            case 0:
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                break;
            case 1:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                break;
            case 2:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                break;
            case 3:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                break;
            case 4:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                break;
            case 5:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                break;
            case 6:
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                break;
            case 7:
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                break;
            case 8:
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                break;
            case 9:
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, false);
                break;
            case 10:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                sleep(3);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                break;
            case 11:
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                sleep(3);
                flashLEDs(chip, leds, false);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                flashLEDs(chip, leds, true);
                break;
            default:
                break;
        }
    }
    gpiod_chip_close(chip);
}

