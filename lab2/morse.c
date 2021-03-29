#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

volatile sig_atomic_t last_signal = 0;

void sig_handler(int sig){
    printf("[%d] received signal %d\n", getpid(), sig);
    last_signal = sig;
}

void short_flash(struct gpiod_line * line, unsigned int line_num){
	int ret;
	ret = gpiod_line_set_value(line, 1);
	if (ret < 0) {
		perror("Set line output failed\n");	
		return;
	}
	printf("Output 0 on line #%u\n", line_num);
	sleep(1);
	ret = gpiod_line_set_value(line, 0);
	if (ret < 0) {
		perror("Set line output failed\n");
		return;
	}
	printf("Output 1 on line #%u\n", line_num);	
}

void long_flash(struct gpiod_line * line, unsigned int line_num){
	int ret;
	ret = gpiod_line_set_value(line, 1);
	if (ret < 0) {
		perror("Set line output failed\n");	
		return;
	}
	printf("Output 0 on line #%u\n", line_num);
	sleep(2);
	ret = gpiod_line_set_value(line, 0);
	if (ret < 0) {
		perror("Set line output failed\n");
		return;
	}
	printf("Output 1 on line #%u\n", line_num);	
}

void show_num(struct gpiod_line * line, unsigned int line_num, int num){
	switch(num){
		case 0: long_flash(line, line_num); sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			break;
		case 1: short_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			break;
		case 2: short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			break;
		case 3: short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			break;
		case 4: short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			break;
		case 5: short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			break;
		case 6: long_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			break;
		case 7: long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			break;
		case 8: long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			break;
		case 9: long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			long_flash(line, line_num);sleep(1);
			short_flash(line, line_num);sleep(1);
			break;
		default:
			break;

	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	char *chipname = "gpiochip0";
	unsigned int line_num = 30;	// GPIO Pin #30
	unsigned int val;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret;
	int score = 0;
	int chances = 3;
	int sec, fir, num_to_show, diode_to_lit;
/*	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}


    // Switches handling
    struct gpiod_line_bulk switches;
    int switch_num[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
    if(gpiod_chip_get_lines(chip, switch_num, 12, &switches)< 0){
        gpiod_chip_close(chip);
        perror("Switch error");
    }
    if(gpiod_line_request_bulk_both_edges_events(&switches, "switch")<0){
        gpiod_chip_close(chip);
        perror("Switch error");
    }
    struct gpiod_line_bulk buttons;
    int button_num[12] = {12,13,14,15,16,17,18,19,20,21,22,23};
    if(gpiod_chip_get_lines(chip, button_num,12,&buttons)<0){
        gpiod_chip_close(chip);
        perror("Button error");
    }
    if(gpiod_line_request_bulk_falling_edge_events(&buttons, "button")<0){
        gpiod_chip_close(chip);
        perror("Button error");
    }*/
    // Switches handling end
    //struct gpiod_line_bulk switches_and_buttons;
    //int sb_num = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
	// LEDs handling
	/*struct gpiod_line_bulk leds;
	int led_num[12] = {24,25,26,27,28,29,30,31};
	if(gpiod_chip_get_lines(chip, led_num, 8, &leds)<0){
		perror("LED error");
		goto close_chip;
	}
	if(gpiod_line_request_bulk_output(&leds, "led", 0)<0){
		perror("LED error");
		goto close_chip;
	}
	int leds_off[8] = {0,0,0,0,0,0,0,0};
	int leds_on[8] = {1,1,1,1,1,1,1,1};
	*/






	/*line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_output(line, CONSUMER, 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		goto release_line;
	}*/

	/* Blink 20 times */
	/*val = 0;
	for (i = 20; i > 0; i--) {
		ret = gpiod_line_set_value(line, val);
		if (ret < 0) {
			perror("Set line output failed\n");
			goto release_line;
		}
		printf("Output %u on line #%u\n", val, line_num);
		sleep(1);
		val = !val;
	}*/
    	static const struct gpiod_line_event EmptyLineEvent;
	static const struct gpiod_line_bulk EmptyLineBulk;
	while(chances > 0){
	sleep(3);
	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}


    // Switches handling
    struct gpiod_line_bulk switches;
    int switch_num[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
    if(gpiod_chip_get_lines(chip, switch_num, 12, &switches)< 0){
        gpiod_chip_close(chip);
        perror("Switch error");
    }
    if(gpiod_line_request_bulk_both_edges_events(&switches, "switch")<0){
        gpiod_chip_close(chip);
        perror("Switch error");
    }
    struct gpiod_line_bulk buttons;
    int button_num[12] = {12,13,14,15,16,17,18,19,20,21,22,23};
    if(gpiod_chip_get_lines(chip, button_num,12,&buttons)<0){
        gpiod_chip_close(chip);
        perror("Button error");
    }
    if(gpiod_line_request_bulk_falling_edge_events(&buttons, "button")<0){
        gpiod_chip_close(chip);
        perror("Button error");
    }




	struct gpiod_line_event last_event = EmptyLineEvent;
    	struct gpiod_line_bulk event_bulk = EmptyLineBulk;
	printf("DEBUUUUG: %d\n", event_bulk.lines[0]);
    	int event_line, switch_value;
    	int waited = 0;
    	int waited_s = 0;
    	struct timespec shortStop = {1,0};
    	struct timespec longStop = {2,0};
		num_to_show = rand() % 24;
		diode_to_lit = (rand() % 8) + 24; 
		// Turn off all the diodes in case something went wrong
		/*if(gpiod_line_set_value_bulk(&leds, leds_off)<0){
			perror("LEDs error");
			goto close_chip;
		}*/
		line = gpiod_chip_get_line(chip, diode_to_lit);
		if(!line){
			perror("Get line failed\n");
			goto close_chip;
		}
		ret = gpiod_line_request_output(line, CONSUMER, 0);
		if(ret < 0){
			perror("Request line as output failed\n");
			goto release_line;
		}
		if(num_to_show < 10){
			show_num(line, diode_to_lit, num_to_show);
		}
		else{
			sec = num_to_show % 10;
			fir = (num_to_show - sec) / 10;
			show_num(line, diode_to_lit, fir);
			show_num(line, diode_to_lit, sec);
		}
        do{
            waited = gpiod_line_event_wait_bulk(&buttons, &longStop, &event_bulk);
            waited_s = gpiod_line_event_wait_bulk(&switches, &longStop, &event_bulk);
            if(waited < 0 || waited_s < 0)
                break;
        }while(waited<=0 && waited_s<=0);
        // bouncing
	int wait_pressed;
	if(waited)
		wait_pressed = waited;
	else
		wait_pressed = waited_s;
        do{
            if(wait_pressed)
                break;
            if(gpiod_line_event_read(event_bulk.lines[0], &last_event)<0){
                perror("Event read error\n");
                goto close_chip;
            }
            wait_pressed = gpiod_line_event_wait(event_bulk.lines[0], &longStop);
        }while(wait_pressed == 1);
	printf("DEBUUUUUG2: %d\n", gpiod_line_offset(event_bulk.lines[0]));
        event_line = gpiod_line_offset(event_bulk.lines[0]);
	printf("Event line got: %d\n", event_line);
        if(num_to_show == event_line){
            printf("Congratulations! You scored a point!\n");
            score++;
            printf("Sum of points: %d\n", score);
            printf("Chances left: %d\n", chances);

	gpiod_line_request_bulk_output(&switches, "switch", 0);
	gpiod_line_request_bulk_output(&buttons, "button", 0);
        }else{
            printf("Unfortunately you missed, you lose a chance\n");
	    printf("Correct answer: %d\n", num_to_show);
            chances--;
            printf("Sum of points: %d\n", score);
            printf("Chances left: %d\n", chances);

	gpiod_line_request_bulk_output(&switches, "switch", 0);
	gpiod_line_request_bulk_output(&buttons, "button", 0);
        }
	sleep(2);
	//gpiod_line_request_bulk_output(&switches, "switch", 0);
	//gpiod_line_request_bulk_output(&buttons, "button", 0);
	gpiod_line_release(line);
	gpiod_chip_close(chip);

	}
	
printf("---------------\n");
printf("|   SUMMARY   |\n");
printf("---------------\n");
printf("Sum of points: %d\n", score);
printf("Chances left: %d\n", chances);
goto end;

release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}
