# Piezo buzzer example code for ESP32-based boards
There is a built-in library provided that allows us to specify a note and it will automatically play the corresponding frequency until you tell it to stop. The method `ledcWriteNote(channel, note, octave)` provides this functionality.
In order to setup the pin, you use `ledcAttachPin(pin, channel)` and we can use channel = 0 for our purposes.
If you wanted to stop the PWM, use `ledcDetachPin(pin)` to remove it.
