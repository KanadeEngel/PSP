PATH = c:/pspsdk/bin

bin2c Launcher660\NPIA00013\EBOOT.PBP  _660launcher.h _660launcher
bin2c RCOFiles660\Install\X000\topmenu_icon.rco _660_install_X_icon.h _660_install_X_icon
bin2c RCOFiles660\Install\X000\topmenu_plugin.rco _660_install_X_plugin.h _660_install_X_plugin
bin2c RCOFiles660\Original\X000\topmenu_icon.rco _660_original_X_icon.h _660_original_X_icon
bin2c RCOFiles660\Original\X000\topmenu_plugin.rco _660_original_X_plugin.h _660_original_X_plugin

make clean
make
rm *.prx *.elf *.SFO *.o *.h
pause
