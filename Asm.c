
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#define MAX_STRLEN 500
#define MAX_LABEL 50
#define MAX_MEM 4096
//project definitions
char* op_codes_names[22][3] = { {"add","0","0x00"}, {"sub","1","0x01"}, {"mul","2","0x02"},{"and","3","0x03"},{"or","4","0x04"},{"xor","5","0x05"},{"sll","6","0x06"},{"sra","7","0x07"}, {"srl","8","0x08"}, {"beq","9","0x09"}, {"bne","10","0x0a"}, {"blt","11","0x0b"}, {"bgt","12","0x0c"}, {"ble","13","0x0d"}, {"bge","14","0x0e"}, {"jal","15","0x0f"},{"lw","16","0x10"}, {"sw","17","0x11"}, {"reti","18","0x12"}, {"in","19","0x13"}, {"out","20","0x14"}, {"halt","21","0x15"}};
char* reg_names[16][3] = { {"$zero","0","0x0"},{"$imm","1","0x1"},{"$v0","2","0x2"},{"$a0","3","0x3"},{"$a1","4","0x4"},{"$a2","5","0x5"},{"$a3","6","0x6"},{"$t0","7","0x7"},{"$t1","8","0x8"},{"$t2","9","0x9"},{"$s0","10","0xa"},{"$s1","11","0xb"},{"$s2","12","0xc"},{"$gp","13","0xd"},{"$sp","14","0xe"},{"$ra","15","0xf"}};
char* word_code = ".word";
//Alot of functions for parsing!
char* wipe_spaces(char* str)  //cleans a string from spare spaces and remove comments (cleans the right section to a # sign)
{
	int break_flag = 0;
	int first_dollar = 0;
	char* t;
	char* new_str;
	char* holder;
	t = str;
	new_str = (char*)malloc(MAX_STRLEN * sizeof(char));
	holder = new_str;
	if (new_str == NULL)
	{
		printf("Error");
	}
	while (*t != '\0')
	{
		if ((*t == '$') && (first_dollar == 0))
		{
			*new_str = ',';
			new_str++;
			first_dollar = 1;
		}
		else if (isspace(*t) != 0)
		{
			t++;
		}
		else if (*t == '#')
		{
			*new_str = '\0';
			break_flag = 1;
			break;
		}
		else
		{
			*new_str = *t;
			t++;
			new_str++;
		}
	}
	if (first_dollar == 1)
	{
		//printf("saw dollar\n");
	}
	if (break_flag != 1)
	{
		*new_str = '\0';
	}
	return holder;
}
//function that will check if a label exist and return the label string (max_len of label is 50)
char* Label_Identifier(char* str) //** here we allocate memory the save a label if there is one
{
	char* t, * label, * r, * holder; //holder holds original adress of the label
	int flag = 0;
	t = str;
	r = str; //t and r are holding base adress of string argument
	label = (char*)malloc((MAX_LABEL + 1) * sizeof(char));
	holder = label;
	while (*t != '\0') // check if there is a format of Label
	{
		if (*t == 58)  //58 is the ascii value of ":"
		{
			flag = 1;
			break;
		}
		t++;
	}
	if (flag == 1)
	{
		while (*r != 58)
		{
			*label = *r;
			label++;
			r++;
		}
		*label = '\0';
	}
	if (flag == 0)
	{
		return NULL;
	}
	else
	{
		return holder; //the function return a pointer to the Label of a current Line
	}
}

char* label_removal(char* str) // this functiosn relies on "Label_Identifer" function to tell if there is a label or not
{							   // if this function being called we assume that there is a label
	char* t, * holder;
	t = str;
	holder = str;
	int flag = 0;
	while (*t != '\0')
	{
		if ((*t != 58) && (flag == 0)) //58 is ascii value of ":"
		{
			t++;
		}
		if ((*t == 58) && (flag == 0)) //58 is ascii value of ":"
		{
			flag = 1;
			t++;
		}
		if (flag == 1)
		{
			*str = *t;
			str++;
			t++;
		}
	}
	*str = '\0';
	return holder;
}

char** parsing_comm(char* str) //** this function assumes she is getting a 'clean command':
{							  //NO spaces,NO labels - only op code, 3 registers and imm with "," between them.

	char** comm_parts;	     //an array that will hold pointers , each pointer points to one compoenet of the command
	int i = 0, j = 0, k = 0, m = 0;
	char* t;
	char** holder;
	t = str;   
	int counter = 0;
	comm_parts =malloc(5 * (sizeof(char*)));
	holder = comm_parts;
	for (i = 0; i < 5; i++)
	{
		*comm_parts = calloc(1,(sizeof(char)) * MAX_STRLEN);
		comm_parts++;
	}
	comm_parts = holder;
	while (*t != '\0')
	{
		if (* t != ',')                    //44 is the ASCII value of psik
		{
			counter++;
			(** comm_parts) = *t;
			//printf("%c ", **comm_parts);
			(*comm_parts)++;
			t++;
		}
		else                      
		{
			//**comm_parts = '\0';
			(*comm_parts) -= counter;
			comm_parts++;
			t++;
			counter = 0;
		}
	}
	*comm_parts -= counter;
	//**comm_parts = '\0';
	comm_parts = holder;
	return comm_parts;
}
int is_only_label(char *str)									  //function that checks if a line is only label (for readability in asmb code)
{
	char* t;
	t = str;
	int flag = 0;
	int return_flag = 0;
	while (*t != '\0')
	{
		if ((return_flag == 1) && (*t != '\0'))
		{
			return_flag = 0;
			break;
		}
		if (flag == 1)
		{
			return_flag = 1;
		}
		if (*t==58)
		{
			flag = 1;
		}
		t++;
	}
	return return_flag;
}
/*
char* dec_to_hex(int dec)
{
	char* hex_num;
	char* holder;
	int i = 0;
	int div = 0;
	hex_num = malloc(8 * sizeof(char));
	holder = hex_num;
	*hex_num = '0';
	hex_num++;
	*hex_num = 'x';
	hex_num++;
	for (i = 4; i > 0; i--)
	{
		div = dec / (pow(16, i));
		if (dec == 0)
		{
			break;
		}
		else if (div == 0 && i!=0)
		{
			*hex_num = '0';
			hex_num++;
		}
		else if (div == 0 && i == 0)
		{
			*hex_num = (div % 16) + 48;
			hex_num++;
			dec = dec - (div % 16);
		}
		else if (div<10 && div>-1)                      //div is number between 0 to 9 so we add 48 for ascii values of numbers
		{
			*hex_num = div + 48;
			hex_num++;
			dec = dec - (div * pow(16, i));
		}
		else
		{
			*hex_num = div + 87;                        //div is number between 10 to 15 so we add 87 to send it to ascii values of a to f
			hex_num++;
			dec = dec - (div * pow(16, i));
		}
	}
	*hex_num = '\0';
	return holder;
}
*/
int is_i_type(char** array)										    //function that checks if a command is i type. if it does it returns the position of the string in the array.
{
	int placement = 0, i = 0, cmp = 0;
	char** t;
	t = array;
	t+=2;
	//printf("%s", *t);
	
	cmp = strcmp(*t, reg_names[1][0]);
	if(cmp==0)
	{
		placement = 2;
	}
	t++;
	cmp = strcmp(*t, reg_names[1][0]);
	if (cmp == 0)
	{
		placement = 3;
	}
	return placement;
	
	//t++;
	//printf("%s\n", *t);
	return i;
}
int is_blank_line(char* str)                                       //function that check if a line is blank line (to skip the line counter) 
{
	char* t;
	int flag = 1;
	t = str;
	while (*t != '\0')
	{
		if ((isspace(*t))== 0)
		{
			flag = 0;
			break;
		}
		t++;
	}
	return flag;
}
int is_hex(char* str)											//this function checks if the immidiate part of the command is in hexa (if it does returns 1 if not retruns 0)
{
	char* t;
	int flag = 1, counter = 0;
	t = str;
	while (*t != '\0')
	{
		if (counter == 0 && *t != '0')
		{
			flag = 0;
			break;
		}
		else if (counter == 1 && *t != 'x')
		{
			flag = 0;
			break;
		}
		counter++;
		t++;
	}
	return flag;
}
int hex_to_dec(char* hex)										// this function converts hexa number to dec (assuming it is hexa number which means 0x in the start)
{																//this functions handle hexa number in capital letter or regular letters!
	int result = 0;
	char* t;
	int i = 0;
	t = hex;
	t += 2;
	for (i = 4; i > -1; i--)
	{
		if (*t > 47 && *t < 58)									// the hexa number is between 0 to 9
		{
			result = result + ((*t - 48) * pow(16, i));
		}
		else if (*t > 96 && *t < 103)
		{
			result = result + ((*t - 87) * pow(16, i));        //the hexa number is between a to f
		}
		else if (*t > 64 && *t < 71)
		{
			result = result + ((*t - 55) * pow(16, i));		   //the hexa number is between A to F
		}
		t++;
	}
	return result;
}
int is_word(char* str)										//this function checks if a command is the special .word command if it does func returns 1 if not returns 0
{
	char* t;
	char* word_pointer;
	int flag = 0;
	t = str;
	word_pointer = word_code;

	while (*word_pointer != '\0')
	{
		if (isspace(*t) != 0)
		{
			while (isspace(*t) != 0)
			{
				t++;
			}
		}
		else if (*word_pointer == *t)
		{
			flag = 1;
			t++;
			word_pointer++;
		}
		else
		{
			flag = 0;
			break;
		}
	}
	return flag;
}
char** word_parsing(char* str)										//we call this function if the function is_word return 1- this fucntion will allocate memory for the adress and constant we want to put in memory
{															// we will use a global array to put the words asside in first run and then write them in the apropriate place in the second run!
	char** word_parts;
	char** word_parts_holder;
	char* t;
	t = str;
	int i = 0,counter=0,flag=0;
	word_parts = malloc(3 * sizeof(char*));
	word_parts_holder = word_parts;
	for (i = 0; i < 3; i++)
	{
		*word_parts = calloc(1, MAX_STRLEN * sizeof(char));
		word_parts++;
	}
	word_parts = word_parts_holder;
	if (isspace(*t) != 0)
	{
		while (isspace(*t) != 0)
		{
			t++;
		}
	}
	while (*t != '\0')                                          //loop that clenas excess spaces in the beggiinng
	{
		if (isspace(*t) == 0)                                  //part of the string which is not spaces
		{
			(**word_parts) = *t;
			t++;
			(* word_parts)++;
			counter++;
		}
		else   //isspace(*t)!=0									we have met a space in the string
		{
			
			while (isspace(*t) != 0)							  // we moving forward in the string to ignore spaces between the relevent fields (as long we keep meeting spaces
			{
				t++;
			}
			(* word_parts)-= counter;
			word_parts++;
			counter = 0;
		}
		
	}
	word_parts = word_parts_holder;
	return word_parts;
}
void main(int argc, char* argv[])
{
	FILE* asmcode;
	FILE* output_file;
	int i = 0,j=0;
	int dec = 0;
	int is_line_is_word = 0;
	char* hex;
	int only_label = 0, i_type_placement = 0, is_blank = 0, is_hexa = 0;												//flag for checking if a line is only a LABEL 
	char* str_check, * label, * no_label;
	char** command_parts;
	char** holder;
	char** word_parts;
	char* wiped_str;
	char* label_table[MAX_MEM] = { 0 };									//a table that will hold a label-line number relation
	int words_table[MAX_MEM] = { 0 };										//a table that every index is a row in the file , every index contains a value the forced to be written!
	str_check = (char*)malloc(MAX_STRLEN * sizeof(char));
	int line_counter=0;												// an intger that will follow the line number of the file (so we can place the line number of a label


	
	//FIRST RUN
	output_file = fopen(argv[2], "w");
	asmcode = fopen(argv[1], "r");
	while (asmcode != NULL)
	{
		str_check = fgets(str_check, MAX_STRLEN, asmcode);			// were holding a line from the the file
		
		if (str_check != NULL)										//safety check for for NULL line
		{
			is_blank = is_blank_line(str_check);					//checking if line is blank.
			if (is_blank == 0)									    //line is not blank!
			{
				is_line_is_word = is_word(str_check);				// we check if a line is the special command .word
				//printf("%d", is_line_is_word);
				if (is_line_is_word == 1)
				{
					word_parts = word_parsing(str_check);
					words_table[atoi(word_parts[1])] = atoi(word_parts[2]);	//we put the word value to be written as integer!
					line_counter++;
					continue;												//we itrate to the next line! (we save word details)
				}
				else  //this line is not a word
				{
					only_label = is_only_label(str_check);           // we check if the next line is made out of label only
					if (only_label == 1)
					{
						label_table[line_counter] = Label_Identifier(str_check);  //we put the label in the index of the line we found it
						continue;												  //we continue to next line without increasing line_counter cause this line contains only a label!
					}
					else  //the line is not only a label
					{
						label = Label_Identifier(str_check);
						if (label == NULL)
						{

							wiped_str = wipe_spaces(str_check);
							printf("%s\n", wiped_str);
							command_parts = parsing_comm(wiped_str);
							line_counter++;
						}
						else //label!=NULL we do have a label in this line
						{
							label_table[line_counter] = label;
							printf("%s", label);
							no_label = label_removal(str_check);
							wiped_str = wipe_spaces(no_label);
							printf("%s\n", wiped_str);
							command_parts = parsing_comm(wiped_str);
							line_counter++;
						}
					}
				}

			}
			else												//line is indeed blank!
			{
				continue;
				//printf("here a blank_line");
				//fprintf(output_file, "%s\n", "here a blank_line");
			}

		}
		else
		{
			break;
		}
	}

	printf("%d\n", line_counter);
	for (i = 0; i < MAX_MEM ; i++)
	{
		if(label_table[i]!=0)
		{
			printf("Line# %d Label - %s\n", i, label_table[i]);
		}
		
		else if (words_table[i] != 0)
		{
			printf("Write the value %d in adress %d", words_table[i], i);
		}
		
	}
	fclose(asmcode);
	fclose(output_file);
	
														
																	
																	
																	
																	
																	
																	
	
	
	//str_check = fgets(str_check, MAX_STRLEN, stdin);
	//label = Label_Identifier(str_check);
	//no_label = label_removal(str_check);
	//wiped_str = wipe_spaces(str_check);
	//printf("%s\n", label);
	//printf("%s\n", no_label);
	//printf("%s\n", wiped_str);
	//label_table[0] = Label_Identifier(str_check);
	//printf("%s", label_table[0]);
	//is_line_is_word = is_word(str_check);
	//printf("%d\n", is_line_is_word);
	//word_parts = word_parsing(str_check);
	//printf("%s", word_parts[1]);
	/*
	for (i = 0; i < 3; i++)
	{
		printf("%s\n", *word_parts);
		word_parts++;
	}
	/*
	is_hexa = is_hex(str_check);
	if (is_hexa == 1)
	{
		dec = hex_to_dec(str_check);
		printf("%d", dec);
	}
	else
	{
		printf("not_hexa");
	}
	*/

	//scanf("%d", &dec);
	//hex = dec_to_hex(dec);
	//printf("%05x", dec);                                               //importent - format of printing decimal number in hexa with width of 5 numbers!
	/*
	output_file = fopen(argv[2],"w");
	asmcode = fopen(argv[1], "r");
	while (asmcode != NULL)
	{
		str_check=fgets(str_check, MAX_STRLEN, asmcode);
		//printf("%d\n", is_blank);
		if (str_check != NULL)
		{
			is_blank = is_blank_line(str_check);
			if (is_blank == 0)									//line is not blank!
			{
				
				//printf("%s\n", str_check);
				//fprintf(output_file, "%s", str_check);
			}
			else												//line is indeed blank!
			{
				continue;
				//printf("here a blank_line");
				//fprintf(output_file, "%s\n", "here a blank_line");
			}

		}
		else
		{
			break;
		}
	}
	fclose(asmcode);
	fclose(output_file);
	for (i = 0; i < 22; i++)
	{
		printf("%s - %s - %s\n", op_codes_names[i][0], op_codes_names[i][1], op_codes_names[i][2]);
	}
	printf("\n\n\n");
	for (j = 0; j < 16; j++)
	{
		printf("%s - %s -%s \n",reg_names[j][0], reg_names[j][1], reg_names[j][2]);
	}
	
	/*
	str_check = fgets(str_check, MAX_STRLEN, stdin);
	only_label = is_only_label(str_check);
	if (only_label == 1)
	{
		printf("only_label\n");
	}
	if (str_check == NULL)
	{
		printf("kaki");
	}
	label = Label_Identifier(str_check);
	if (label != NULL)
	{
		printf("%s\n", label);
		no_label = label_removal(str_check);
		wiped_str = wipe_spaces(no_label);
		command_parts = parsing_comm(wiped_str);
	}
	else
	{
		printf("nolabel\n");
		//printf("%s\n", wiped_str);
		wiped_str = wipe_spaces(str_check);
		command_parts = parsing_comm(wiped_str);
	}
	holder = command_parts;
	for (i = 0; i < 5; i++)										// test loop for contents of the array 
	{
		printf("%s\n", *holder);
		holder++;
	}
	//i_type_placement = is_i_type(command_parts);
	//printf("%d\n",i_type_placement);
	//printf("%d", atoi(reg_names[4][1]));
	//free(str_check);
	//free(label);
	/*
	i = 0;
	for (i = 0; i < 5; i++)
	{
		free(command_parts[i]);
	}
	*/

}