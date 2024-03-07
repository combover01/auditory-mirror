alsa_out -j teensy1 -d hw:CARD=TEENSY2 -r 44100 -p 4096 -n 2 >/dev/null &
alsa_out -j teensy2 -d hw:CARD=TEENSY3 -r 44100 -p 4096 -n 2 >/dev/null &
