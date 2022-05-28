import simpleaudio as sa

def playsound(name):
    try:
        w_object = sa.WaveObject.from_wave_file(name)
        p_object = w_object.play()
        print(str(name) + " is playing")
        p_object.wait_done()
        print("Finished")
    except FileNotFoundError:
        print("File doesn't exist")

keys = ['c_minor', 'c_major', 'cs_major']

for key in keys:
    for it in range(1,8):
        playsound("scale_" + str(key) + str(it) + ".wav")
