This is a font used for library [lcdgfx](https://github.com/lexus2k/lcdgfx).

It is made from [zpix](https://github.com/SolidZORO/zpix-pixel-font) by using a small program bdf2lcdgfx(The code of this tool is also put in this repo).

To use the tool, you can run:
```shell
gcc -o bdf2lcdgfx bdf2lcdgfx.c
./bdf2lcdgfx <bdfFileName> <fontName>
```

The main part of the font is made by the tool. However, to use the font, you should add some other lines.

The code of the tool may looks very ugly, since I don't want to put too much energy into making such a font.
