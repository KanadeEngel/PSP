PATH = c:/pspsdk/bin

bin2c FILES\ICON0.PNG  _icon_install.h _icon_install
bin2c FILES\PARAM.SFO  _param_install.h _param_install
bin2c FILES\SECURE.BIN _data_install.h _data_install


make clean
make
rm *.prx *.elf *.SFO *.o *.h
pause