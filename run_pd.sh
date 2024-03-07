qjackctl
cd /home/hack/Documents/NEWMIRRORFOLDER/auditory-mirror && lxterminal -e python pi_receive_sensors.py
pd -verbose -jack -r 44100 -outchannels 8 -noadc -nomidi -blocksize 2048 -gui /home/hack/Documents/NEWMIRRORFOLDER/auditory-mirror/jrubin_PD_Files/oscReceive.pd

