#!/usr/bin/python
import gpiod
import time
import simpleaudio as sa

def single_event(num, timeout):
    event = 0
    new = False
    ev_line = num.event_wait(sec=timeout)
    while ev_line:
        new = True
        event = num.event_read()
        ev_line = num.event_wait(0, 150000000)
    if new == False:
        return event
    else: 
        return event.type

# return codes for bulk event
def bulk_event(num, timeout):
    event = 0 
    result = num.event_wait(sec=timeout)
    if result != None:
        line = result[0]
        read_val = line.event_read()
        oldevent = read_val.type
        event = single_event(line, timeout)
        if event == 0:
            event = oldevent
        if event == 2 and str(line) == str(num.to_list()[0]):
            event = 12
        elif event == 2 and str(line) == str(num.to_list()[1]):
            event = 13
        elif event == 2 and str(line) == str(num.to_list()[2]):
            event = 14
        elif event == 2 and str(line) == str(num.to_list()[3]):
            event = 15
        elif event == 2 and str(line) == str(num.to_list()[4]):
            event = 16
        elif event == 2 and str(line) == str(num.to_list()[5]):
            event = 17
        elif event == 2 and str(line) == str(num.to_list()[6]):
            event = 18
        elif event == 2 and str(line) == str(num.to_list()[7]):
            event = 19
    return event

# source: https://linuxhint.com/play_sound_python
def playsound(name):
    try:
        w_object = sa.WaveObject.from_wave_file(name)
        p_object = w_object.play()
        print(str(name) + " is playing")
        p_object.wait_done()
        print("Finished")
    except FileNotFoundError:
        print("File " + str(name) + " doesn't exist")


zeroflag = False
oneflag = False
twoflag = False

chip = gpiod.Chip('9008000.gpio')
# switches and buttons offsets
switch_num = [0, 1, 2]
button_num = [12, 13, 14, 15, 16, 17, 18, 19]

switches = chip.get_lines(switch_num)
buttons = chip.get_lines(button_num)

switches.request(consumer="its_me", type=gpiod.LINE_REQ_EV_BOTH_EDGES)
buttons.request(consumer="its_me", type=gpiod.LINE_REQ_EV_BOTH_EDGES)

while True:
    # checking if scale has changed
    event = bulk_event(switches, 1)
    if event == 12:
        zeroflag = not zeroflag
    elif event == 13:
        oneflag = not oneflag
    elif event == 14:
        twoflag = not twoflag
    # checking if button (key) was pressed
    event_but = bulk_event(buttons, 1)
    if zeroflag == True and event != 0:
        playsound("/usr/bin/dzwieki/scale_c_minor"+str(event-11)+".wav")
    if oneflag == True and event != 0:
        playsound("/usr/bin/dzwieki/scale_c_major"+str(event-11)+".wav")
    if twoflag == True and event != 0:
        playsound("/usr/bin/dzwieki/scale_cs_major"+str(event-11)+".wav")

