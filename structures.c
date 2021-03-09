//
//  structures.c
//
//
//  Set of solutions to questions:
//  1. Store data for multiple Students and pull the ones with certain ages or odd Student number
//  2. Compare 2 date structures
//  3. Add and subtract complex numbers using a structures
//  4. Convert Fahrenheit to Celsius
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//function declarations
void question1();
void question2();
void question3();
void question4();

void main()
{
    int qnum = 1;
    printf("Please enter the number of the question you would like to start with (1-4):\n");
choose1:
    scanf("%d", &qnum);
    switch (qnum)
    {
    default:
        printf("Please enter a valid number between 1 and 4\n");
        goto choose1;
        break;
    case 1:
        question1();
        break;
    case 2:
        question2();
        break;
    case 3:
        question3();
        break;
    case 4:
        question4();
        break;
    }
}
void question1()
{
    //------------Q1------------//
    //*** NOTE: f1.txt is already created

    FILE *file_1, *file_2, *file_3;
    file_1 = fopen("f1.txt", "r");
    file_2 = fopen("f2.txt", "w");
    char text2[20] = "File 2 text";
    for (int i = 0; i < strlen(text2); i++)
    {
        putc(text2[i], file_2);
    }
    fclose(file_2);
    file_2 = fopen("f2.txt", "r");
    file_3 = fopen("f3.txt", "w");

    char c;

    while ((c = getc(file_1)) != EOF)
    {
        putc(c, file_3);
    }
    while ((c = getc(file_2)) != EOF)
    {
        putc(c, file_3);
    }

    fclose(file_1);
    fclose(file_2);
    fclose(file_3);

    //next question?
    char choice = 'y';
    printf("Ready for question 2? (enter y or n)\n");
    scanf(" %c", &choice);
    if (choice == 'y')
    {
        question2();
    }
    else if (choice == 'n')
    {
        question1();
    }
}
void question2()
{
    //------------Q2------------//
    //*** NOTE: fileQ2.txt is already created

    FILE *file1;
    char ch;

    file1 = fopen("fileQ2.txt", "r");
    printf("\nOutput is: \n");
    while ((ch = getc(file1)) != EOF)
    {
        printf("%c\n", ch);
    }

    fclose(file1);

    //next question?
    char choice = 'y';
    printf("\nReady for question 3? (enter y or n)\n");
    scanf(" %c", &choice);
    if (choice == 'y')
    {
        question3();
    }
    else
    {
        question2();
    }
}
void question3()
{
    //------------Q3------------//
    FILE *cfile;
    char ch;
    cfile = fopen("a8.c", "r");
    printf("\nGet ready, this is going to be big... File contains: \n\n");
    while ((ch = getc(cfile)) != EOF)
    {
        printf("%c", ch);
    }

    //next question?
    char choice = 'y';
    printf("Ready for question 4? (enter y or n)\n");
    scanf(" %c", &choice);
    if (choice == 'y')
    {
        question4();
    }
    else
    {
        question3();
    }
}
void question4()
{
    //------------Q4------------//

    printf("Please enter a string of characters, digits, and symbols (up to 20 in length).\n");
    int i = 0;
    char array[20], ch, garbage;
    garbage = getchar();
    while (ch != '\n')
    {
        ch = getchar();
        array[i] = ch;
        i++;
    }

    FILE *newFile;

    newFile = fopen("newfileQ4.txt", "w");
    fputs(array, newFile);

    fclose(newFile);

    //next question?
    char choice = 'y';
    printf("Want to return to menu? (enter y or n)\n");
    scanf(" %c", &choice);
    if (choice == 'y')
    {
        main();
    }
    else
    {
        printf("\nQuitting –- Have a nice day!\n\n");
    }
}
