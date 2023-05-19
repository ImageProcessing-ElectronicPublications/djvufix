# djvufix

## djvufixdpi

djvufixdpi - is a utility that changes DPI value in one bundled DjVu (single- or multyple-paged) file to a desired value.

The new DPI is assigned to the INFO chunk.

Syntax:
```shell
djvufixdpi djvufile <n>
```
where `<n>` is the new DPI value (must be between 25 and 2400).

## djvufixrotate

djvufixrotate is a command-line utility for correcting the wrong orientation in the bundled DjVu-files.

djvufixrotate checks the orientation bit on every DjVu page in a multypage DjVu-file and if it finds that "orientation=0" it corrects it to "orientation=1".

djvufixrotate fixes an old DjVuLibre bug: http://djvu.cvs.sourceforge.net/viewvc/djvu/djvulibre-3.5/libdjvu/DjVuInfo.cpp?r1=1.11&r2=1.12&sortby=rev

Syntax:
```shell
djvufixrotate djvufile
```

## Authors

This utility is made as a modified version of the [DjVuVersion utility](http://www.djvu.org/files/DjVuVersion.zip).

Author: monday2000@yandex.ru

07.04.2010.
