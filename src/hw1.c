#include "hw1.h"
#include "const.h"
#include <stdlib.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the program
 * and will return a unsigned short (2 bytes) that will contain the
 * information necessary for the proper execution of the program.
 *
 * IF -p is given but no (-r) ROWS or (-c) COLUMNS are specified this function
 * MUST set the lower bits to the default value of 10. If one or the other
 * (rows/columns) is specified then you MUST keep that value rather than assigning the default.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return Refer to homework document for the return value of this function.
 */
unsigned short validargs(int argc, char **argv) {
    int rows;
    int shiftRows = 0;
    int cols;
    int alreadyR = 0;
    int alreadyK = 0;
    int alreadyC = 0;

    int value = 0x0000;
    if(argc == 0 || argc == 1){
        return value;
    }
    if(isEquals(*(argv+1), "-h")){
        value = 0x8000;
        return value;
    }
    else{
        value = value & 0x0000;//SET THE MSB TO 0
        if(isEquals(*(argv+1), "-f") || isEquals(*(argv+1), "-p")){//CHECK IF FIRST ARGUMENT IS VALID
            if(isEquals(*(argv+1), "-f")){//FRACTIONATED MORSE CIPHER
                value = value | 0x4000; //SET THE SECOND MSB TO 1
            }
            if(isEquals(*(argv+1), "-p")){//POLYBIUS CIPHER
                value = value | 0x0000; //SET THE SECOND MSB TO 0.
                value = value | 0xA0; //SET BITS 7 TO 4 BY DEFAULT TO DECIMAL 10 (CONV TO BINARY).
                value = value | 0xA; //SET BITS 3 TO 0 BY DEFAULT TO DECIMAL 10 (CONV TO BINARY).
                rows = 10;
                cols = 10;
            }
        }
        else{ //WHEN THE FIRST ARGUMENT IS INVALID...
            value = 0x0000;
            return value;
        }

        if(argc == 2 && (isEquals(*(argv+1), "-f") || isEquals(*(argv+1), "-p"))){ //TOO FEW ARGUMENTS BECAUSE -p OR -f CIPHERS NEED -e OR -d ARGS.
            value = 0x0000;
            return value;
        }

        if(isEquals(*(argv+2), "-d") || isEquals(*(argv+2), "-e")){//CHECK THE SECOND ARGUMENT FOR VALIDITY
            if(isEquals(*(argv+2), "-d")){
                value = value | 0x2000; //SET THE THIRD MSB TO 1
            }
            if(isEquals(*(argv+2), "-e")){
                value = value | 0x0000; //SET THE THIRD MSB TO 0
            }
        }
        else{ //WHEN THE SECOND ARGUMENT IS INVALID...
            value = 0x0000;
            return value;
        }
        for(int i = 3; i < argc; i++){ //CHECK IN ARRAY OF ARGUMENTS STARTING FROM THE ELEMENT AFTER LAST POSITIONAL ARGUMENT
                                   //THIS IS BECAUSE OPTIONAL ARGUMENTS CAN BE PLACED INTO ANY ORDER.

            if(isEquals(*(argv+1), "-f") && (isEquals(*(argv + i), "-c") || isEquals(*(argv + i), "-r"))){ //CHECK IF ARG i IS -C B/C LAST ARGS CAN BE IN ANY ORDER
                //INCORRECT ARGS FOR SPECIFIED CIPHER. -f CANNOT HAVE ROWS AND COLS SPECIFIED.
                value = 0x0000;
                return value;
            }

            if(isEquals(*(argv + i), "-k")){ //CHECK IF ARG i IS -k B/C LAST ARGS CAN BE IN ANY ORDER
                //CHECK IF KEY IS VALID WHERE THE KEY SHOULD BE argv[i++] (NEXT ELEMENT/STRING)

                if(isEquals(*(argv+1), "-p")){

                    if(isRepeats(*(argv + i + 1)) || isContains(polybius_alphabet, *(argv + i + 1)) == 0){
                        value = 0x0000; //INVALID KEY
                        return value;
                    }
                    if(alreadyK == 1){
                        value = 0x0000;
                        return value;
                    }
                    else{
                        key = *(argv + i + 1);//ASSIGN TO key GLOBAL VARIABLE IN const.h
                        alreadyK = 1;
                    }
                }

                if(isEquals(*(argv+1), "-f")){

                    if(isRepeats(*(argv + i + 1))  || isContains(fm_alphabet, *(argv + i + 1)) == 0){
                        value = 0x0000; //INVALID KEY
                        return value;
                    }
                    if(alreadyK == 1){
                        value = 0x0000;
                        return value;
                    }
                    else{
                        key = *(argv + i + 1);//ASSIGN TO key GLOBAL VARIABLE IN const.h
                        alreadyK = 1;
                    }
                }
                //IF THE KEY IS INVALID, VALUE = 0 AND RETURN 0
            }
            if(isEquals(*(argv+1), "-p") && isEquals(*(argv + i), "-r")){ //CHECK IF ARG i IS -r B/C LAST ARGS CAN BE IN ANY ORDER
                rows = atoi(*(argv + i + 1)); //ENCODE NUMBER OF ROWS. INT ROWS TO BINARY FOR BITS 7 TO 4.
                if(rows < 9 || rows > 15){ //IF THE ROWS IS INVALID, VALUE = 0 AND RETURN 0 (MORE THAN 15 OR LESS THAN 9)
                    value = 0x0000;
                    return value;
                }
                if(alreadyR == 1){
                    value = 0x0000;
                    return value;
                }
                else{ //OTHERWISE CARRY ON SETTING BITS 7 TO 4 WITH ROWS
                    value = value & 0xE00F; //CLEAR THE BITS 7 TO 4 OF THE DEFAULT SET 10. KEEP THE FIRST 3 BITS BECAUSE IMPORTANT.
                    shiftRows = rows << 4; //SHIFT ROWS LEFT 4 BITS. NEEDS TO BE SET FOR BITS 7 TO 4.
                    value = value | shiftRows; //SET THE BITS.
                    alreadyR = 1;
                }

            }
            if(isEquals(*(argv+1), "-p") && isEquals(*(argv + i), "-c")){ //CHECK IF ARG i IS -C B/C LAST ARGS CAN BE IN ANY ORDER
                cols = atoi(*(argv + i + 1)); //ENCODE THE NUMBER OF COLUMNS. INT COLS TO BINARY FOR BITS 3 TO 0.
                if(cols < 9 || cols > 15){ //IF THE COLS IS INVALID, VALUE = 0 AND RETURN 0 (MORE THAN 15 OR LESS THAN 9)
                    value = 0x0000;
                    return value;
                }
                if(alreadyC == 1){
                    value = 0x0000;
                    return value;
                }
                else{ //OTHERWISE CARRY ON SETTING BITS 3 TO 0 WITH COLS.
                    value = value & 0xE0F0; //CLEAR THE BITS 3 TO 0 OF THE DEFAULT SET 10. KEEP THE FIRST 3 BITS BECAUSE IMPORTANT.
                    value = value | cols; //SET THE BITS.
                    alreadyC = 1;
                }
            }

            if(isEquals(*(argv + i), "-e") || isEquals(*(argv + i), "-d")){ //WHEN THE THIRD ARGUMENT IS INVALID...
                value = 0x0000;
                return value;
            }

            // else{
            //     if(isEquals(*(argv+1), "-p") && isEquals(*(argv + i), "-r") && alreadyR == 1){
            //         value = 0x0000;
            //         return value;
            //     }

            //     if(isEquals(*(argv+1), "-p") && isEquals(*(argv + i), "-c") && alreadyC == 1){
            //         value = 0x0000;
            //         return value;
            //     }

            //     if(isEquals(*(argv + i), "-k") && alreadyK == 1){
            //         value = 0x0000;
            //         return value;
            //     }
            // }
        }

        //GET LENGTH OF THE POLYBIUS ALPHABET BECAUSE IT CAN BE DIFFERENT.

        int p_alphabet_length = 0;
        char *polybius_alphabet_tracker = polybius_alphabet;
        while(*polybius_alphabet_tracker != '\0'){
            polybius_alphabet_tracker++; //INCREMENT POINTER.
            p_alphabet_length++;
        }


        if(((rows * cols) < p_alphabet_length) && isEquals(*(argv+1), "-p")){
            value = 0x0000;
            return value;//RETURN 0 BECAUSE INVALID ARGUMENT
        }
    }

    return value;
}

int isEquals(char *char1, char *char2){ //COMPARES EACH CHARACTER OF THE ARRAY OF CHARACTERS(STRING)
                                        //OF THE CURRENT STRING IN INDEX i OF CHARACTER ARRAY (ARRAY OF STRINGS) argv
    while(*char1 != '\0'){//WHILE THE CHARACTER IS NOT EQUAL TO THE STRING TERMINATOR
        if(*char1 > *char2){
            return 0;
        }
        if(*char1 < *char2){
            return 0;
        }
        char1++; //INCREMENT POINTER OF FIRST CHARACTER IF THEY ARE THE SAME
        char2++; //INCREMENT POINTER OF THE SECOND CHARACTER IF THEY ARE THE SAME
    }
    if(*char2 != '\0'){//IF THE COMPARING STRING POINTER HAS YET TO REACH THE END OF THE STRING BUT THE
                       //STRING BEING COMPARED TO HAS REACHED THE TERMINATOR,
                       //THAT MEANS THEY ARE NOT THE SAME.
        return 0;
    }

    return 1; //THEY ARE THE SAME.

}

int isRepeats(char *char1){
    int length = 0;
    int incrementor = 0;
    while(*(char1 + incrementor) != '\0'){ //GET THE SIZE OF THE KEY BY LOOPING THROUGH THE KEY.
        length++;
        incrementor++;
    }


    //COMPARE ONE CHARACTER WITH EVERY OTHER CHARACTER.
    for(int i = 0; i < length; i++){
        for(int j = i+1; j < length; j++){
            if(*(char1 + i) == *(char1 + j)){
                return 1; //THERE IS A REPEATED CHARACTER. THUS, INVALID KEY.
            }
        }
    }

    return 0; //THERE ARE NO REPEATED CHARACTERS. THUS, IT IS A VALID KEY.
}

int isContains(const char *alphabet, char *char1){
    const char *alphabet_original = alphabet; //SET A SECOND POINTER TO KEEP ORIGINAL POSITION TO THE START OF THE ALPHABET.

    while(*alphabet != '\0'){//WHILE THE VALUE OF THE CHARACTER IN THE ALPHABET STRING IS NOT NULL
        if(*alphabet == *char1){ //IF THE CURRENT CHAR IN ALPHABET AND CURRENT CHAR IN KEY ARE EQUAL, THEN THE KEY IS CONTAINED IN THE ALPHABET.
            char1++; //MOVE ON TO COMPARE THE NEXT CHARACTER IN THE KEY.
            alphabet = alphabet_original; //RESET THE ALPHABET POINTER BACK TO THE BEGIN TO RESTART NEW COMPARISON.
        }
        else{
            alphabet++;
        }

        if(*char1 == '\0'){ //IF THE KEY IS POINTED TO NULL CHARACTER VALUE, THAT MEANS THE ENTIRE KEY HAS BEEN COMPARED AND ALL
                            //CHAR VALUES IN THE STRING ARE IN THE ALPHABET, SINCE THE CHAR POINTER IS ONLY INCREMENTED WHEN THERE IS A MATCH.
            return 1;
        }
    }
    //ONCE THE POINTER REACHES THE END OF THE ALPHABET, THAT MEANS THE KEY CONTAINS A CHARACTER THAT IS NOT IN THE ALPHABET.
    return 0;
}
