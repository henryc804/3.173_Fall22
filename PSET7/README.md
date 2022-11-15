## PSET 7 scripts
The data_collection script contains code to output over serial sensor data from 2 capacitive sensors at specifically 50 Hz (samples per second) for 1.5 seconds each. You should feel free to change the pin declarations and sampling rate/overall time, but you will need to make sure you change the corresponding variables. Also, make sure you the updated time and sampling rate gets carried over to your training on Edge Impulse and also on your live_inference script.

static_buffer script should already be provided by your own example code when importing your Edge Impulse generated library.

live_inference script is custom written, and you will need to make sure the ```include``` statement that you use is changed to match the name of the E.I. generated library that you use. Also, make sure your sampling rate and sample time matches what you've trained your system on.