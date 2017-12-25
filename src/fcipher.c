#include "fcipher.h"
#include "const.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int fEncode(unsigned short mode){

    /*  THERE IS NO NEED TO EXPLICITLY CREATE A POINTER FOR "morse_table"
        BECAUSE "morse_table" IS ALREADY THE POINTER TO THE ARRAY WHEN INITIALIZED IN const.
        *(morse_table) PROVIDES DEREFERENCE OF "morse_table" POINTER, WHICH IS THE CHAR POINTER
        OF THE CURRENT ELEMENT THAT "morse_table" IS POINTING TO. ITERATING IT WOULD ITERATE THE CHAR POINTER
        OF THE CURRENT ELEMENT "morse_table" POINTS TO
        WHICH IS EQUIVALENT TO *(*(morse_table)++), NOT *(morse_table++); */

    char *pointerFMKey = fm_key;
    char *pointerFMKey_original = fm_key;
    char *morseBuffer = polybius_table;
    char *morseBuffer_original = polybius_table;

    createFMKey(); //GENERATE THE FRACTIONATED KEY USING A USER INPUT KEY FOR THE CIPHER.

    char character = '\0';
    const char *morseCharacter;

    int count = 0;//KEEP TRACK OF HOW MANY WHITE SPACES, TABS, NEWLINES. MAKES SURE "xx" IS ONLY OUTPUTTED ONCE

    int morseCounter = 0;
    while(character != EOF){

        character = getchar();

        if(character == EOF){
            break;
        }

        if(character == ' '){//IF WHITESPACE, IT IS THE END OF A WORD. IGNORE MULTIPLE WHITE SPACES
            count++;
            if(count == 1){

                    if(morseCounter == 3){ //OUTPUT THE CIPHERTEXT EVERY THREE MORSE CHARACTERS AND RESET THE POINTER OF THE BUFFER TO ADD THE REST.
                    //OUTPUT
                        morseBuffer = morseBuffer_original;
                        morseCounter = 0;
                        // morseCounter++;
                        char cipherCharacter = toCipherText(morseBuffer);
                        printf("%c", cipherCharacter);
                        morseBuffer = morseBuffer_original;
                    }
                    *(morseBuffer) = 'x';
                    morseBuffer++;
                    morseCounter++;
                    // if(morseCounter == 4){ //YOU ONLY WANT TO ADD XX AFTER A WORD, NOT BEFORE A WORD. (i.e: TAB INDENTATION)
                    //     morseCounter = 0;
                    //     *(morseBuffer) = 'x'; //ONLY ADD ONE EXTRA x SINCE AFTER EVERY PLAINTEXT CHARACTER, AN X IS ALREADY SET. THIS WOULD MAKE xx.
                    //     morseBuffer++;
                    //     morseCounter++;
                    // }


            }
        }



        if(character ==  '\t'){//IF TAB, IT IS THE END OF A WORD. IGNORE MULTIPLE WHITE SPACES
            count++;
            if(count == 1){

                if(morseCounter == 3){ //OUTPUT THE CIPHERTEXT EVERY THREE MORSE CHARACTERS AND RESET THE POINTER OF THE BUFFER TO ADD THE REST.
                //OUTPUT
                    morseBuffer = morseBuffer_original;
                    morseCounter = 0;
                    // morseCounter++;
                    char cipherCharacter = toCipherText(morseBuffer);
                    printf("%c", cipherCharacter);
                    morseBuffer = morseBuffer_original;
                }
                *(morseBuffer) = 'x';
                morseBuffer++;
                morseCounter++;
                // if(morseCounter == 4){ //YOU ONLY WANT TO ADD XX AFTER A WORD, NOT BEFORE A WORD. (i.e: TAB INDENTATION)
                //     morseCounter = 0;
                //     *(morseBuffer) = 'x'; //ONLY ADD ONE EXTRA x SINCE AFTER EVERY PLAINTEXT CHARACTER, AN X IS ALREADY SET. THIS WOULD MAKE xx.
                //     morseBuffer++;
                //     morseCounter++;
                // }

            }
        }



        if(character == '\n'){//IF NEWLINE, IT IS THE END OF A WORD. IGNORE MULTIPLE WHITE SPACES
            //count++;
            //if(count == 1){
                // morseBuffer = morseBuffer_original;
                // morseCounter = 0;
                // count = 0;

                if(morseCounter == 2 || morseCounter == 0){
                    *(morseBuffer) = 'x';
                    morseCounter++;
                }

                //TRUNCATE THE 'x' TO PREVENT NEWLINE FROM PRINTING EXTRA 'x' FOR INVALID ENCODE
                if(morseCounter == 3){ //OUTPUT THE CIPHERTEXT EVERY THREE MORSE CHARACTERS AND RESET THE POINTER OF THE BUFFER TO ADD THE REST.
                    //OUTPUT
                    morseBuffer = morseBuffer_original;
                    morseCounter = 0;
                    char cipherCharacter = toCipherText(morseBuffer);
                    printf("%c", cipherCharacter);
                    morseBuffer = morseBuffer_original;
                }
                printf("%c", '\n');
                morseCounter = 0;
                morseBuffer = morseBuffer_original;
                // if(*(morseBuffer-1) != 'x'){
                //     *(morseBuffer) = 'x'; //ONLY ADD ONE EXTRA x SINCE AFTER EVERY PLAINTEXT CHARACTER, AN X IS ALREADY SET. THIS WOULD MAKE xx.
                // }
                // morseBuffer++;
                // morseCounter++;
            //}
        }




        if(character != ' ' && character != '\t' && character != '\n'){

            count = 0;//RESET THE COUNTER TO 0 SINCE THE CURRENT CHARACTER BEING ENCODED IS NOT A WHITE SPACE, PREVENTING
                      //EXCESS x's FROM BEING ENCODED TO MORSE.

            //IF "character" is not explicitly set to '\0', the following line creates -1 '\377' for "character" and does not allow input to be read.
            int morse_table_index = character - 0x21; //'!' HAS THE HEX ENCODING OF 0x21.
                                                      //TO CONVERT THAT TO ITS INDEX IN THE MORSE TABLE (WHICH IS 0), SUBTRACT ITS VALUE
                                                      //THIS WILL BE USED AS THE BASELINE/STARTING INDEX.

            morseCharacter = *(morse_table + morse_table_index); //WHY DONT I HAVE TO RESET mose_table POINTER??


            if(morseCharacter == NULL || *(morseCharacter) == '\0'){
                //EXIT FAILURE. FAILED KEY. NOT IN MORSE CODE.
                return 1;
            }

            /*
                1) ADD TILL 3 MORSE CHARACTERS ARE IN THE BUFFER
                2) OUTPUT CONVERSION
                3) RESET BUFFER POINTER TO START
                4) IF morseCharacter CONTAINS MORE CHARACTERS, CONTINUE UNTIL STRING TERMINATOR.
                5) CONTINUE TO GET NEXT morseCharacter FROM USER STRING INPUT
                6) STEP 1
                ***IF END OF CHARACTER, ADD 'x' TO THE BUFFER
                ***IF SPACE, TAB, NEWLINE, ADD AN ADDITIONAL 'x' TO BUFFER(BECAUSE AT THE END OF PLAINTEXT CHAR, AN x IS ALREADY THERE)
                ***morseCounter MAKES SURE THAT THE CIPHERCHARACTER IS OUTPUTTED EVERY THREE MORSECODE CHARACTERS.
            */

            while(*(morseCharacter) != '\0'){
                if(morseCounter < 3){
                    *(morseBuffer) = *(morseCharacter);
                    morseBuffer++;
                    morseCharacter++;
                    morseCounter++;
                }
                if(morseCounter == 3){ //OUTPUT THE CIPHERTEXT EVERY THREE MORSE CHARACTERS AND RESET THE POINTER OF THE BUFFER TO ADD THE REST.
                    //OUTPUT
                    morseBuffer = morseBuffer_original;
                    morseCounter = 0;
                    char cipherCharacter = toCipherText(morseBuffer);
                    printf("%c", cipherCharacter);
                    morseBuffer = morseBuffer_original;
                }
                if(*(morseCharacter) == '\0'){ //IT IS THE END OF THE MORSE TRANSLATION FOR THAT LETTER. CREATE DIVIDER.
                    *(morseBuffer) = 'x';
                    morseBuffer++;
                    morseCounter++;
                }
            }

        }

    }

    return 0;
}



int fDecode(unsigned short mode){
    char *pointerFMKey = fm_key;
    char *pointerFMKey_original = fm_key;
    char *morseBuffer = polybius_table;
    char *morseBuffer_original = polybius_table;

    createFMKey(); //GENERATE THE FRACTIONATED KEY USING A USER INPUT KEY FOR THE CIPHER.

    char character = '\0';
    const char *morseCharacter;
    char previousFMorseCharacter = '\0';
    char backup = 0;


    while(1){
        if(backup == 0){
            character = getchar();
        }
        else{
            character = backup;
        }
        // if(character == EOF){
        //     break;
        // }
        char upcomingCharacter = getchar();
        backup = upcomingCharacter;

        if(character == EOF){
            morseBuffer = morseBuffer_original;
            if(*(morseBuffer) != '\0'){
                toPlainText(morseBuffer); //PRINT OUT REMAINING CHARACTER IF EXISTS.
            }
            morseBuffer = morseBuffer_original;
            break;
        }

        if(character == '\n'){
            morseBuffer = morseBuffer_original;

            if(*(morseBuffer) != '\0'){
                toPlainText(morseBuffer); //PRINT OUT REMAINING CHARACTER IF EXISTS.
            }

            printf("%c", '\n');

            //CLEAR TO PREVENT MISOUTPUT OR SEGFAULTS
            while(*(morseBuffer) != '\0'){
                *(morseBuffer) = '\0';
                morseBuffer++;
            }
            morseBuffer = morseBuffer_original;
        }

        else{

            int index = 0;
            while(*(pointerFMKey) != '\0'){
            //GET THE INDEX OF THE CHARACTER IN THE FMKEY

                if(*(pointerFMKey + index) == character){
                    break; //ONCE A MATCH IS FOUND BETWEEN THE CHARACTER AND THE KEY
                }
                index++;

            }

            const char *fractionatedMorse = *(fractionated_table + index);

            while(fractionatedMorse != NULL && *(fractionatedMorse) != '\0'){
            //FILL THE BUFFER UNTIL AN 'x' IS REACHED. THIS IS BECAUSE 'x' INDICATES END OF LETTER OR WORD.


                if(*(fractionatedMorse) != 'x'){
                    //IF THE FRACTIONATED MORSE CHARACTER IS NOT 'x', THEN ADD IT TO THE BUFFER TO BE TRANSLATED
                    *(morseBuffer) = *(fractionatedMorse);
                    morseBuffer++;
                    previousFMorseCharacter = *(fractionatedMorse);
                    fractionatedMorse++;

                }

                if(*(fractionatedMorse) == '\n'){
                    printf("%c", '\n');
                }

                else{
                    //OTHERWISE THE FRACTIONATED MORSE CHARACTER IS AN 'x'
                    //IF IT IS A SINGLE 'x', LOOK FOR THE LETTER IN THE PLAINTEXT MORSE TABLE.
                    //IF IT IS A DOUBLE 'x', ADD A SPACE.

                    if(*(fractionatedMorse) == 'x'){

                        // if(upcomingCharacter != EOF){
                        //     backup = upcomingCharacter;
                            if(upcomingCharacter == '\n' && previousFMorseCharacter == 'x'){
                                printf("\n");
                                backup = 0;
                            }
                        // }


                        if(previousFMorseCharacter == 'x' && upcomingCharacter != '\n'){ //IT IS A DOUBLE 'x' BUT SPLIT BETWEEN TWO FRACTIONATED MORSE CODES.
                            printf("%c", ' ');
                            morseBuffer = morseBuffer_original;
                            fractionatedMorse++;
                        }
                        else{
                            //THE END OF A WORD OR CHARACTER
                            morseBuffer = morseBuffer_original;
                            if(*(morseBuffer) != '\0'){
                                toPlainText(morseBuffer);
                            }
                            previousFMorseCharacter = *(fractionatedMorse);
                            fractionatedMorse++;


                            //CLEAR THE BUFFER TO PREVENT WRONG OUTPUT OR SEGFAULTS. IF 4 CHARACTERS PREVIOUSLY, BUT 3 CHARACTERS NEXT, THERE WILL BE 1 EXTRA CHARACTER, CAUSING ERROR.
                            morseBuffer = morseBuffer_original;
                            while(*(morseBuffer) != '\0'){
                                *(morseBuffer) = '\0';
                                morseBuffer++;
                            }
                            morseBuffer = morseBuffer_original;

                            // //IF AFTERWARDS CONTAINS ANOTHER 'x' THAT MEANS IT'S THE END OF A WORD, NOT JUST A CHARACTER.
                            // if(*(fractionatedMorse + 1) == 'x'){
                            //     printf("%c", ' ');
                            // }
                        }

                    }

                }

            }

        }

    }

    return 0;

}


char toCipherText(char *morseBuffer){
    char *morseBuffer_original = morseBuffer;
    char *pointerFMKey = fm_key;

    for(int i = 0; i < 26; i++){
        if(isFMEquals(morseBuffer, *(fractionated_table + i))){
            morseBuffer = morseBuffer_original;
            return *(pointerFMKey + i);
        }
    }

    return '\0';
}

void toPlainText(char *morseBuffer){
    char *morseBuffer_original = morseBuffer;

    int index = 0;
    int track = 1;
    char plain = '\0';

    while(track){
        if(isFMEquals(morseBuffer, *(morse_table + index))){
            plain = index + 0x21;
            printf("%c", plain);
            morseBuffer = morseBuffer_original;
            track = 0;
            break;
        }
        index++;
    }

}


void createFMKey(){
    char *pointerFMKey = fm_key;
    char *pointerFMKey_original = fm_key;
    const char *fm_alphabet_original = fm_alphabet;

    const char *key_original = key;
    if(key != NULL){
        while(*key != '\0'){//PLACE THE KEY LETTERS FIRST.
            *(pointerFMKey) = *(key);
            key++;
            pointerFMKey++;
        }
        //THEN PLACE THE REST. IF THERE IS NO USER ENTERED KEY,
        //MAKE SURE THERE IS NO REPEATED CHARACTER PLACED FROM THE KEY.
        key = key_original;

        while(*(fm_alphabet) != '\0'){
            //CHECK IF THE CURRENT LETTER AT THE fm_alphabet POINTER IS PART OF THE USER ENTERED KEY.
            //IF IT IS, DONT ADD IT TO AVOID DUPES.
            if(matchesWithFMKey(fm_alphabet)){
                fm_alphabet++; //MOVE TO THE NEXT CHARACTER IN THE ALPHABET
            }

            else{//IF THE LETTER IS NOT PART OF THE KEY, ADD IT TO THE FMKey.
                *(pointerFMKey) = *(fm_alphabet);
                pointerFMKey++;
                fm_alphabet++;
            }
        }
    }
    else{ //KEY IS NULL. THE USER DID NOT ENTER A KEY. THE FMKey ARRAY IS JUST A COPY OF THE fm_alphabet
        while(*(fm_alphabet) != '\0'){
            *(pointerFMKey) = *(fm_alphabet);
            pointerFMKey++;
            fm_alphabet++;
        }
    }

    pointerFMKey = pointerFMKey_original;
    fm_alphabet = fm_alphabet_original;

}

int matchesWithFMKey(const char *alphabet){
    const char *key_original = key; //KEEP THE ORIGINAL POINTER OF THE KEY FOR LATER USE.

    while(*(key) != '\0'){

        if(*(alphabet) == *(key)){ //IS CURRENT LETTER IN THE ALPHABET IN THE KEY?
            key = key_original;
            return 1; //THE CURRENT LETTER IN THE ALPHABET MATCHES WITH THE LETTER IN THE KEY.
                      //DO NOT PUT IN THE TABLE BECAUSE IT IS ALREADY PLACED IN THERE
        }
        key++; //IF NOT, COMPARE THE NEXT LETTER IN THE KEY WITH THE CURRENT LETTER IN THE ALPHABET
    }

    key = key_original;
    return 0; //NO MATCH.
}

int isFMEquals(const char *char1, const char *char2){ //COMPARES EACH CHARACTER OF THE ARRAY OF CHARACTERS(STRING)
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