#include "pcipher.h"
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

void createPTable(unsigned short mode){

    int rows = mode & 0xF0; //MASK EVERYTHING BUT THE ROW NUMBER IN MODE
    rows = rows >> 4; //SHIFT 4 BITS TO THE RIGHT
    int cols = mode & 0xF; //MASK EVERYTHING BUT THE COL NUMBER IN MODE

    char *polybius_table_original = polybius_table;//KEEP THE STARTING ADDRESS OF THE polybius_table POINTER.
    char *pointerPTable = polybius_table;//EXPLICITLY CREATE A POINTER TO POINT TO THE BUFFER ARRAY. B/C YOU CANT INCREMENT THE ARRAY ITSELF. YOU CAN ONLY INCREMENT THE POINTER.
    const char *key_original = key; //KEEP THE ORIGINAL POINTER OF THE KEY FOR LATER USE.

    char *polybius_alphabet_original = polybius_alphabet;

    while(key != NULL && *key != '\0'){
        *(pointerPTable) = *(key);
        key++;
        pointerPTable++;
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            key = key_original; //RESET THE KEY POINTER BECAUSE IT WILL BE REUSED LATER.


            if(*(polybius_alphabet) == '\0'){ //IF THE ALPHABET IS FINISHED AND THERE IS STILL SPACE REMAINING (I.E: THE FOR LOOP STILL NEEDS TO CONTINUE)
                *(pointerPTable) = '\0'; //FILL THE REST WITH NULL CHARACTERS.
                pointerPTable++;
            }

            else{

                while(*(polybius_alphabet) != '\0'){

                    if(key != NULL && matchesWithKey(polybius_alphabet)){ //IF IT MATCHES, SKIP OVER THE LETTER.
                        polybius_alphabet++;
                    }

                    else{//OTHERWISE ADD THE LETTER FROM THE ALPHABET TO THE TABLE AND MOVE ON TO THE NEXT LETTER IN THE ALPHABET.
                        *(pointerPTable) = *(polybius_alphabet); //SET THE VALUE OF POLYBIUS ALPHABET WHERE POINTER IS CURRENTLY
                                                                  //POINTING TO INTO THE POLYBIUS TABLE BUFFER AT THE POISITION WHERE
                                                                  //THE TABLE POINTER IS POINTING TO.
                        pointerPTable++;
                        polybius_alphabet++;
                    }
                }
            }

        }
    }

    pointerPTable = polybius_table_original;
    polybius_alphabet = polybius_alphabet_original;
}


int pEncode(unsigned short mode){

    char *polybius_alphabet_original = polybius_alphabet;
    char *polybius_table_original = polybius_table;//KEEP THE STARTING ADDRESS OF THE polybius_table POINTER.
    char *pointerPTable = polybius_table;//EXPLICITLY CREATE A POINTER TO POINT TO THE BUFFER ARRAY. B/C YOU CANT INCREMENT THE ARRAY ITSELF. YOU CAN ONLY INCREMENT THE POINTER.

    int rows = mode & 0xF0; //MASK EVERYTHING BUT THE ROW NUMBER IN MODE
    rows = rows >> 4; //SHIFT 4 BITS TO THE RIGHT
    int cols = mode & 0xF; //MASK EVERYTHING BUT THE COL NUMBER IN MODE

    int coordinate = 0;
    int table_index = 0;

    //printf("Enter String to be Encoded:");
    // char *toBeEncoded = "";
    char character;
    while(character != EOF){
        character = getchar();
        // makeString(toBeEncoded, character);

        if(character == ' '){//IF WHITESPACE, PRINT WHITESPACE
            printf(" ");
        }
        if(character == '\t'){//IF TAB, PRINT TAB
            printf("\t");
        }
        if(character == '\n'){
            printf("\n");
        }

        while(*(pointerPTable) != '\0'){
            if(character == *(pointerPTable)){
                //GET THE COORDINATE OF THE CHARACTER IN THE TABLE FROM THE ALPHABET INDEX.
                int row_cipher = table_index / cols;
                int col_cipher = table_index % cols;
                printf("%X", row_cipher);
                printf("%X", col_cipher);
                break;
            }
            table_index++;
            pointerPTable++;
        }

        if(character != ' ' && character != '\t' && character != '\n' && character != EOF){
            if(*pointerPTable == '\0'){
                return 1;
            }
        }

        table_index = 0;
        pointerPTable = polybius_table_original; //RESTART ALPHABET POINTER TO MATCH THE NEXT LETTER TO ENCODE.
    }



    // while(*input != '\0'){

    //     if(*input == *polybius_alphabet){
    //         //GET THE COORDINATE OF THE CHARACTER IN THE TABLE FROM THE ALPHABET INDEX.
    //         int row_cipher = alphabet_index / cols;
    //         int col_cipher = alphabet_index % cols;
    //         makeString();
    //         polybius_alphabet = polybius_alphabet_original; //RESTART ALPHABET POINTER TO MATCH THE NEXT LETTER TO ENCODE.
    //         input++;
    //     }

    //     alphabet_index++;
    //     polybius_alphabet++;
    // }
    return 0;

}




void pDecode(unsigned short mode){
    int rowToSearch = 0;
    int colToSearch = 0;
    int counter = 1;
    int printCounter = 0;

    int rows = mode & 0xF0; //MASK EVERYTHING BUT THE ROW NUMBER IN MODE
    rows = rows >> 4; //SHIFT 4 BITS TO THE RIGHT
    int cols = mode & 0xF; //MASK EVERYTHING BUT THE COL NUMBER IN MODE

    //printf("Enter String to be Decoded:");

    char character;

    while(character != EOF){
        character = getchar();

        if(character == ' ' || character == '\t' || character == '\n'){
            if(character == ' '){//IF WHITESPACE, PRINT WHITESPACE
                printf(" ");
                counter = 1; //RESET COUNTER SO IT WON'T LOSE TRACK. NEXT FIRST NUMBER IS ROW AGAIN.
            }

            if(character == '\t'){//IF TAB, PRINT TAB
                printf("\t");
                counter = 1; //RESET COUNTER SO IT WON'T LOSE TRACK. NEXT FIRST NUMBER IS ROW AGAIN.
            }

            if(character == '\n'){
                printf("\n");
                counter = 1;
            }
        }

        else{
            if(counter%2 != 0){//ITS ROW NUMBER B/C OUTPUT IS ROW, COL
                counter++;
                if(character == 'A'){
                    rowToSearch = 10;
                }
                if(character == 'B'){
                    rowToSearch = 11;
                }
                if(character == 'C'){
                    rowToSearch = 12;
                }
                if(character == 'D'){
                    rowToSearch = 13;
                }
                if(character == 'E'){
                    rowToSearch = 14;
                }
                if(character == 'F'){
                    rowToSearch = 15;
                }
                if(character != 'A' && character != 'B' && character != 'C' && character != 'D' && character != 'E' && character != 'F'){
                    rowToSearch = character - '0';
                }
                printCounter++;
            }
            else{
                if(counter%2 == 0){//ITS COL NUMBER B/C OUTPUT IS ROW, COL
                    counter++;
                    if(character == 'A'){
                        colToSearch = 10;
                    }
                    if(character == 'B'){
                        colToSearch = 11;
                    }
                    if(character == 'C'){
                        colToSearch = 12;
                    }
                    if(character == 'D'){
                        colToSearch = 13;
                    }
                    if(character == 'E'){
                        colToSearch = 14;
                    }
                    if(character == 'F'){
                        colToSearch = 15;
                    }
                    if(character != 'A' && character != 'B' && character != 'C' && character != 'D' && character != 'E' && character != 'F'){
                        colToSearch = character - '0';
                    }
                    printCounter++;
                }
            }

            if(printCounter == 2){
                printCounter = 0;
                polybiusFind(rowToSearch, colToSearch, rows, cols);
            }
        }
    }
}


int matchesWithKey(char *alphabet){
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

void polybiusFind(int rowToSearch, int colToSearch, int rows, int cols){

    char *pointerPTable = polybius_table;
    int i = 0;
    int j = 0;

    //LOOP THROUGH THE TABLE. IF THEY MATCH, PRINT THE LETTER LOCATED AT THAT INDEX.
    while(i < rows){
        while(j < cols){
            if((i == rowToSearch) && (j == colToSearch)){
                printf("%c", *pointerPTable);
                return;
            }
            j++;
            pointerPTable++;
        }
        i++;
        j = 0;
    }
}



// void makeString(char *string, char toAppend){
//     int length = 0;
//     int incrementor = 0;
//     while(*(string + incrementor) != '\0'){ //GET THE LENGTH OF THE STRING BY LOOPING THROUGH.
//         length++;
//         incrementor++;
//     }

//     *(string + incrementor) = toAppend;
//     *(string + incrementor + 1) = '\0';
// }