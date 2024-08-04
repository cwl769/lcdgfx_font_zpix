#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

char buf[1024];
char op[1024];
// char tmp[1024];
char map[16][16];

size_t getWord(const char * src, char * tar)
{
    size_t len = 0;
    for(;*src!=' ' && *src != '\n';++src,++tar)
    {
        *tar = *src;
        ++len;
    }
    *tar = '\0';
    return len;
}

int main(int argc, char *argv[])
{
    FILE * bdf = fopen(argv[1], "r");
    if(bdf == NULL)exit(-1);
    FILE * output = fopen("output", "w");
    if(output == NULL)exit(-1);

    int size = 0;
    int chars = 0;

    while(fgets(buf, sizeof(buf), bdf) != NULL)
    {
        // printf("%s", buf);
        size_t oplen = getWord(buf, op);
        if(strcmp(op, "SIZE")==0)
        {
            sscanf(buf+5, "%d", &size);

            size = 16;

            fprintf(output, "const uint8_t %s%dx%d[] PROGMEM = \n{\n", argv[2], size, size);
            fprintf(output, "0x01, %#04x, %#04x, 0x00,\n", size, size);
        }
        else if(strcmp(op, "CHARS")==0)
        {
            sscanf(buf+6, "%d", &chars);

            for(int cnt=0;cnt<chars;++cnt)
            {
                fgets(buf, sizeof(buf), bdf);
                uint16_t unicode;
                sscanf(buf+12, "%hX", &unicode);

                fgets(buf, sizeof(buf), bdf);
                fgets(buf, sizeof(buf), bdf);
                fgets(buf, sizeof(buf), bdf);

                fgets(buf, sizeof(buf), bdf);
                int width, height, dx, dy;
                sscanf(buf, "%*s%d%d%d%d", &width, &height, &dx, &dy);
                printf("%d %d %d %d [%d, %d]\n", width, height, dx, dy, 9-dy-height, 9-dy);

                fgets(buf, sizeof(buf), bdf);

                memset(map, 0x00, sizeof(map));
                for(int i=9-dy-height;i<16;++i)
                {
                    fgets(buf, sizeof(buf), bdf);
                    getWord(buf, op);
                    if(strcmp(op, "ENDCHAR")==0)
                        break;
                    uint16_t data;
                    sscanf(buf, "%hX", &data);
                    for(int j=0;j<16;++j)
                        map[i][16-j] = ((data>>j)&1);
                    // assert(i<=12);
                }
                printf("U+%04X\n", unicode);
                for(int i=0;i<16;++i)
                {
                    for(int j=0;j<16;++j)
                    {
                        putchar( (map[i][j])? '#' : ' ' );
                    }
                    putchar('\n');
                }

                fprintf(output, "%#04x, %#04x, %#04x,\n", unicode>>8, unicode&0xff, 0x01);
                for(int i=0;i<2;++i)
                {
                    for(int j=0;j<16;++j)
                    {
                        uint8_t byte = 0;
                        for(int k=i*8+8-1;k>=i*8;--k)
                            byte = ( (byte<<1) | (uint8_t)map[k][j] );
                        fprintf(output, "%#04x, ", byte);
                    }
                }
                fprintf(output, "// U+%04X\n", unicode);
            }
        }
    }
    fprintf(output, "0x00, 0x00, 0x00\n");
    fprintf(output, "};");

    fclose(bdf);
    fclose(output);
    return 0;
}