#include <stdlib.h>

#include "hw1.h"
#include "debug.h"
#include "pcipher.h"
#include "fcipher.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int main(int argc, char **argv)
{
    unsigned short mode;

    mode = validargs(argc, argv);

    debug("Mode: 0x%X", mode);

    if(mode == 0x8000) {
        USAGE(*argv, EXIT_SUCCESS);
        return EXIT_SUCCESS;
    }

    if(mode == 0x0000){
        USAGE(*argv, EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    else{ //EITHER A POLYBIUS CIPHER OR A FRACTIONATED CIPHER
        if((mode & 0x4000) == 0x4000){ //CHECK IF SECOND MSB IS SET. IF SET, IT IS AN -f CIPHER

            //CALL -f CIPHER HELPER METHOD IN SEPARATE C FILE.
            if((mode & 0x2000) == 0x2000){ //CHECK IF THIRD MSB IS SET. IF SET, THEN IT IS A -d DECODE.
                //IT IS A DECODE.
                int result = fDecode(mode);
                if(result == 1){
                    return EXIT_FAILURE;
                }
                else{
                    return EXIT_SUCCESS;
                }
            }
            else{
                //IT IS AN ENCODE.
                int result = fEncode(mode);

                if(result == 1){
                    return EXIT_FAILURE;
                }
                else{
                    return EXIT_SUCCESS;
                }

            }

        }
        else{ //OTHERWISE IT HAS TO BE A -p CIPHER

            //CALL -p CIPHER HELPER METHOD IN SEPARATE C FILE.

            createPTable(mode);

            if((mode & 0x2000) == 0x2000){ //CHECK IF THIRD MSB IS SET. IF SET, THEN IT IS A -d DECODE.
                //IT IS A DECODE.
                pDecode(mode);
            }
            else{
                //IT IS AN ENCODE.
                int result = pEncode(mode);
                if(result == 1){
                    return EXIT_FAILURE;
                }
                else{
                    return EXIT_SUCCESS;
                }
            }
        }

        // printf("%d\n", mode);
        // printf("%s\n", key);
    }

}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */