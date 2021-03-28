#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

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
	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

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
	while(chances > 0){
		num_to_show = rand() % 24;
		diode_to_lit = (rand() % 8) + 24; 
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
		// TODO: getting input

		// TODO: scoring and displaying result


	}
	


release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}
