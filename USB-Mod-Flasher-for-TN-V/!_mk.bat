PATH = c:/pspsdk/bin

bin2c RCOFiles660\Install\X000\topmenu_icon.rco _660_install_X_icon.h _660_install_X_icon
bin2c RCOFiles660\Original\X000\topmenu_icon.rco _660_original_X_icon.h _660_original_X_icon

make clean
make
rm *.prx *.elf *.SFO *.o *.h
pause
