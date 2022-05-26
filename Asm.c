
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX_STRLEN 500
#define MAX_LABEL 50
#define MAX_MEM 4096
//project definitions
char* op_codes_names[22][3] = { {"add","0","00"}, {"sub","1","01"}, {"mul","2","02"},{"and","3","03"},{"or","4","04"},{"xor","5","05"},{"sll","6","06"},{"sra","7","07"}, {"srl","8","08"}, {"beq","9","09"}, {"bne","10","0A"}, {"blt","11","0B"}, {"bgt","12","0C"}, {"ble","13","0D"}, {"bge","14","0E"}, {"jal","15","0F"},{"lw","16","10"}, {"sw","17","11"}, {"reti","18","12"}, {"in","19","13"}, {"out","20","14"}, {"halt","21","15"}};
char* reg_names[16][3] = { {"$zero","0","0"},{"$imm","1","1"},{"$v0","2","2"},{"$a0","3","3"},{"$a1","4","4"},{"$a2","5","5"},{"$a3","6","6"},{"$t0","7","7"},{"$t1","8","8"},{"$t2","9","9"},{"$s0","10","A"},{"$s1","11","B"},{"$s2","12","C"},{"$gp","13","D"},{"$sp","14","E"},{"$ra","15","F"}};
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
/*
int is_i_type(char** array)										    //function that checks if a command is i type. if it does it returns the position of the string in the array.
{
	int placement = 0, i = 0, cmp = 0;
	char** t;
	t = array;
	
	cmp = strcmp(t[2], reg_names[1][0]);
	if(cmp==0)
	{
		placement = 2;
	}
	//t++;
	cmp = strcmp(t[3], reg_names[1][0]);
	if (cmp == 0)
	{
		placement = 3;
	}
	return placement;
	
	//t++;
	//printf("%s\n", *t);
	return i;
}
*/
int is_i_type(char** array)										    //function that checks if a command is i type. if it does it returns the position of the string in the array.
{
	int flag = 0, i = 0;
	int optional = 0;
	int reg_cmp = 1;
	int op_cmp=1;
	char** t;
	t = array;
	for (i = 0; i < 22; i++)
	{
		op_cmp = strcmp(t[0], op_codes_names[i][0]);					//checking in which index we get the equality of op codes 
		if (op_cmp == 0)
		{
			if (((i > -1) && (i < 9)) || (i==16) || (i==19))	//op code is one of the arithmetic options or lw or in we check rs,rt
			{
				reg_cmp = strcmp(t[2], reg_names[1][0]);        //check if rs equal imm
				if (reg_cmp == 0)
				{
					flag = 1;
				}
				reg_cmp = strcmp(t[3], reg_names[1][0]);       //check if rt equal imm
				if (reg_cmp == 0)
				{
					flag = 1;
				}
			}
			else if (i == 15)
			{
				reg_cmp = strcmp(t[2], reg_names[1][0]);
				if (reg_cmp == 0)
				{
					flag = 1;
				}
			}
			else if ((i > 8 && i < 15) || i == 17 || i == 20)
			{
				reg_cmp = strcmp(t[1], reg_names[1][0]);        //check if rd equal imm
				if (reg_cmp == 0)
				{
					flag = 1;
				}
				reg_cmp = strcmp(t[2], reg_names[1][0]);        //check if rs equal imm
				if (reg_cmp == 0)
				{
					flag = 1;
				}
				reg_cmp = strcmp(t[3], reg_names[1][0]);       //check if rt equal imm
				if (reg_cmp == 0)
				{
					flag = 1;
				}
			}
		}
	}
	return flag;
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
	int flag = 0, counter = 0;
	t = str;
	if (t[0] == '0' && t[1] == 'x')
	{
		flag = 1;
	}
	/*
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
	*/
	return flag;
}
void force_upper_hex(char* str)								//this function takes a hexa string and forces it as a lowercase hexa string (also removes the 0x for the written file)
{
	char* t;
	char* r;
	t = str;
	r = str;
	t += 2;
	while (*t!= '\0')
	{
		if (isupper(*t) != 0 || isdigit(*t)!=0)									//current char is uppercase
		{
			*r = *t;
			r++;
			t++;
		}
		else
		{
			*r = (* t) - 32;
			r++;
			t++;
		}
	}
	*r = '\0';
	return;
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
			
			while (isspace(*t) != 0)							  // we are moving forward in the string to ignore spaces between the relevent fields (as long we keep meeting spaces
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
int is_label_imm_func(char* str)
{
	int flag = 0;
	char* t;
	t = str;
	if (isalpha(*t) != 0)
	{
		flag = 1;
	}
	return flag;
}

void main(int argc, char* argv[])
{
	FILE* asmcode;
	FILE* output_file;
	FILE* asm_2nd_run;
	int op = 0, rd = 0, rs = 0, rt = 0;
	int i = 0, j = 0, k = 0;;
	int dec = 0;
	int is_line_is_word = 0;
	int write_flag = 0;
	int final_line = 0;
	int is_label_imm = 0;
	char* hex;
	int second_run=0;
	int only_label = 0, i_type_flag = 0, is_blank = 0, is_hexa = 0;												//flag for checking if a line is only a LABEL 
	char* str_check, * label, * no_label;
	char** command_parts;
	char** holder;
	char** word_parts;
	char* wiped_str;
	char* label_table[MAX_MEM] = { 0 };									//a table that will hold a label-line number relation
	int words_table[MAX_MEM] = { 0 };										//a table that every index is a row in the file , every index contains a value the forced to be written!
	str_check = (char*)malloc(MAX_STRLEN * sizeof(char));
	int line_counter=0;	
	int str_cmp = 0;
	int zero = 0;
	char zeros[6] = { '0','0','0','0','0','\0'};
	int atoi_idx = 0;
											// an intger that will follow the line number of the file (so we can place the line number of a label


	
	//FIRST RUN
	asmcode = fopen(argv[1], "r");
	while (fgets(str_check, MAX_STRLEN, asmcode)!= NULL)
	{
																	// were holding a line from the the file
		
		if (str_check != NULL)										//safety check for for NULL line
		{
			is_blank = is_blank_line(str_check);					//checking if line is blank.
			if (is_blank == 0)									    //line is not blank!
			{
				is_line_is_word = is_word(str_check);				// we check if a line is the special command .word
				printf("is the line a word? %d\n", is_line_is_word);
				if (is_line_is_word == 1)
				{
					word_parts = word_parsing(str_check);
					words_table[atoi(word_parts[1])] = atoi(word_parts[2]);		//we put the word value to be written as integer!
					printf("%s %s %s\n",word_parts[0], word_parts[1], word_parts[2]);
					if (atoi(word_parts[1]) > final_line)
					{
						final_line = atoi(word_parts[1]);
					}
					
					for (k = 0; k < 3; k++)
					{
						free(word_parts[k]);
					}
					free(word_parts);
					//line_counter++;
					is_line_is_word = 0;
																//we itrate to the next line! (we save word details)
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
							printf("the wiped str is %s", wiped_str);
							command_parts = parsing_comm(wiped_str);
							i_type_flag = is_i_type(command_parts);
							printf("   is i type ? - %d\n", i_type_flag);
							if (i_type_flag == 1)
							{
								line_counter+=2;
							}
							else
							{
								line_counter++;
							}
							for (k = 0; k < 5; k++)
							{
								free(command_parts[k]);
							}
							free(command_parts);
							free(wiped_str);
							free(label);
						}
						else //label!=NULL we do have a label in this line
						{
							label_table[line_counter] = label;
							printf("the label is %s", label);
							no_label = label_removal(str_check);
							wiped_str = wipe_spaces(no_label);
							printf("the wiped str %s", wiped_str);
							command_parts = parsing_comm(wiped_str);
							i_type_flag = is_i_type(command_parts);
							printf("   is i type ? - %d\n", i_type_flag);
							if (i_type_flag == 1)
							{
								line_counter += 2;
							}
							else
							{
								line_counter++;
							}
							for (k = 0; k < 5; k++)
							{
								free(command_parts[k]);
							}
							free(command_parts);
							free(wiped_str);
						}
					}
				}

			}
			else												//line is indeed blank!
			{
				//continue;
				//printf("here a blank_line");
				//fprintf(output_file, "%s\n", "here a blank_line");
			}

		}
		else
		{
			break;
		}
		//memset(str_check, 0, MAX_STRLEN);
	}
	printf("current line counter is - %d\n", line_counter);
	for (i = 0; i < MAX_MEM ; i++)
	{
		if(label_table[i]!=0)
		{
			printf("Line# %d Label - %s\n", i, label_table[i]);
		}
		
		else if (words_table[i] != 0)
		{
			printf("Write the value %d in adress %d\n", words_table[i], i);
		}
		
	}
	if (line_counter > final_line)
	{
		final_line = line_counter;
	}
	printf("final line is - %d\n", final_line);
	//free(str_check);
	fclose(asmcode);
	
	printf("***************   SECOND RUN NOW  *********************\n");
	//SECOND RUN
	line_counter = 0;
	output_file = fopen(argv[2], "w");
	asm_2nd_run = fopen(argv[1], "r");
	while (fgets(str_check, MAX_STRLEN, asm_2nd_run)!=NULL)
	{														// were holding a line from the the file
		printf("Current line is - %d\n", line_counter);
		if (str_check != NULL)										//safety check for for NULL line
		{
			is_blank = is_blank_line(str_check);					//checking if line is blank.
			if (is_blank == 0)									    //line is not blank!
			{
				is_line_is_word = is_word(str_check);
				if (words_table[line_counter] != 0)					//we check if there is a word to write in the current line!
				{
					fprintf(output_file, "%05X\n", words_table[line_counter] & 0xfffff);		//if there is a word we write in hexadecimal format and keep going on
					//printf("%X", words_table[line_counter] & 0xfffff);
					line_counter++;
											//we itrate to the next line! (we save word details)
				}
				else if (is_line_is_word == 1)
				{
					continue;
				}
				else												//there is no word to write in the current line
				{
					only_label = is_only_label(str_check);           // we check if the next line is made out of label only
					if (only_label == 1)
					{
						continue;												  //if its onlylabel line we skip cause we allready handeld it
					}
					else  //the line is not only a label
					{
						label = Label_Identifier(str_check);
						if (label == NULL)
						{

							wiped_str = wipe_spaces(str_check);
							printf("the wiped str - %s\n", wiped_str);
							command_parts = parsing_comm(wiped_str);
							i_type_flag = is_i_type(command_parts);
							//printf("	is i type ? - %d\n", i_type_flag);
							for (op = 0; op < 22; op++)
							{
								if (strcmp(command_parts[0],op_codes_names[op][0]) == 0)
								{
									printf("op idx - %d\n", op);
									break;
									
								}
							}
							for (rd = 0; rd < 16; rd++)
							{
								if (strcmp(command_parts[1], reg_names[rd][0]) == 0)
								{
									printf("rd idx - %d\n", rd);
									break;
								}
							}
							for (rs = 0; rs < 16; rs++)
							{
								if (strcmp(command_parts[2], reg_names[rs][0]) == 0)
								{
									printf("rs idx - %d\n", rs);
									break;
								}
							}
							for (rt = 0; rt < 16; rt++)
							{
								if (strcmp(command_parts[3], reg_names[rt][0]) == 0)
								{
									printf("rt idx - %d\n", rt);
									break;
								}
							}
							if (i_type_flag == 1)													//print to file when in type command
							{
								printf("i type flag %d\n", i_type_flag);
								printf("KIDUD - %s%s%s%s\n", op_codes_names[op][2], reg_names[rd][2], reg_names[rs][2], reg_names[rt][2]);
								fprintf(output_file, "%s%s%s%s\n", op_codes_names[op][2], reg_names[rd][2], reg_names[rs][2], reg_names[rt][2]);
								printf("command_parts[4] is - %s\n", command_parts[4]);
								is_hexa = is_hex(command_parts[4]);
								printf("is hexa? %d\n", is_hexa);
								if (is_hexa == 1)													//we check if the immidate is in hexa format
								{
									force_upper_hex(command_parts[4]);
									fprintf(output_file, "%s\n", command_parts[4]);
								}
								else     //imm is not in hexa (either label or decimal)
								{
									is_label_imm = is_label_imm_func(command_parts[4]);
									printf("is imm label? %d\n", is_label_imm);
									if (is_label_imm == 1)		    //we check if the imm value is a LABEL						
									{
										for (j = 0; j < MAX_MEM; j++)
										{
											if (label_table[j] == NULL)
											{
												continue;
											}
											else
											{
												str_cmp = strcmp(label_table[j], command_parts[4]);
												printf("str_cmp =%d\n", str_cmp);
												if (str_cmp == 0)
												{
													
													printf("the label found in label table indx - %d\n", j);
													printf("IMM KIDUD - %05X\n", (j & 0xfffff));
													fprintf(output_file, "%05X\n", (j & 0xfffff));
													break;
												}
											}
										}
									}
									else   //command_parts[4] is a decimal value
									{
										printf("command_parts[4] in string mode -  %s\n", command_parts[4]);
										atoi_idx = atoi(command_parts[4]);
										printf("command_parts[4] in after atoi - %d\n", atoi_idx);
										if (atoi_idx == 0)
										{

											fprintf(output_file, "%s\n", zeros);
										}
										else
										{
											printf("IMM KIDUD - %05X\n", (atoi(command_parts[4])) & 0xfffff);
											fprintf(output_file, "%05X\n", (atoi(command_parts[4])) & 0xfffff);
										}
									}


								}
								line_counter += 2;
								for (k = 0; k < 5; k++)
								{
									free(command_parts[k]);
								}
								free(command_parts);
								free(wiped_str);
							}
							else       //print to file when NOT in i type command
							{
								printf("R type comm %s%s%s%s\n", op_codes_names[op][2], reg_names[rd][2], reg_names[rs][2], reg_names[rt][2]);
								fprintf(output_file, "%s%s%s%s\n", op_codes_names[op][2], reg_names[rd][2], reg_names[rs][2], reg_names[rt][2]);
								line_counter++;
								for (k = 0; k < 5; k++)
								{
									free(command_parts[k]);
								}
								free(command_parts);
								free(wiped_str);
							}

						}
						else //label!=NULL we do have a label in this line
						{
							no_label = label_removal(str_check);
							wiped_str = wipe_spaces(no_label);
							//printf("%s", wiped_str);
							command_parts = parsing_comm(wiped_str);
							i_type_flag = is_i_type(command_parts);
							//printf("	is i type ? - %d\n", i_type_flag);
							for (op = 0; op < 22; op++)
							{
								if (strcmp(command_parts[0], op_codes_names[op][0]) == 0)
								{
									break;
								}
							}
							for (rd = 0; rd < 16; rd++)
							{
								if (strcmp(command_parts[1], reg_names[rd][0]) == 0)
								{
									break;
								}
							}
							for (rs = 0; rs < 16; rs++)
							{
								if (strcmp(command_parts[2], reg_names[rs][0]) == 0)
								{
									break;
								}
							}
							for (rt = 0; rt < 16; rt++)
							{
								if (strcmp(command_parts[3], reg_names[rt][0]) == 0)
								{
									break;
								}
							}
							if (i_type_flag == 1)
							{
								fprintf(output_file, "%s%s%s%s\n", op_codes_names[op][2], reg_names[rd][2], reg_names[rs][2], reg_names[rt][2]);
								is_hexa = is_hex(command_parts[4]);
								if (is_hexa == 1)
								{
									force_upper_hex(command_parts[4]);
									fprintf(output_file, "%s\n", command_parts[4]);
									for (k = 0; k < 5; k++)
									{
										free(command_parts[k]);
									}
									free(command_parts);
									free(wiped_str);
								}
								else
								{
									is_label_imm = is_label_imm_func(command_parts[4]);
									if (is_label_imm!=0)
									{
										for (j = 0; j < MAX_MEM; j++)
										{
											if (label_table[j] == NULL)
											{
												continue;
											}
											else
											{
												str_cmp = strcmp(label_table[j], command_parts[4]);
												printf("str_cmp =%d\n", str_cmp);
												if (str_cmp == 0)
												{
													
													printf("the label found in label table indx - %d\n", j);
													printf("IMM KIDUD - %05X\n", (j & 0xfffff));
													fprintf(output_file, "%05X\n", j & 0xfffff);
													break;
												}
											}
										}
									}
									printf("command_parts[4] in string mode - %s\n", command_parts[4]);
									atoi_idx = atoi(command_parts[4]);
									printf("command_parts[4] after atoi - %d\n", atoi_idx);
									if (atoi_idx == 0)
									{
										printf("atoi is zero!!!!! fking zeroooo");
										fprintf(output_file, "%s\n", zeros);
									}
									else
									{
										printf("IMM KIDUD - %05X\n", (atoi(command_parts[4])) & 0xfffff);
										fprintf(output_file, "%05X\n", (atoi(command_parts[4])) & 0xfffff);
									}
									for (k = 0; k < 5; k++)
									{
										free(command_parts[k]);
									}
									free(command_parts);
									free(wiped_str);
								}
								line_counter += 2;
							}
							else
							{
								fprintf(output_file, "%s%s%s%s\n", op_codes_names[op][2], reg_names[rd][2], reg_names[rs][2], reg_names[rt][2]);
								line_counter++;
								for (k = 0; k < 5; k++)
								{
									free(command_parts[k]);
								}
								free(command_parts);
								free(wiped_str);
							}
						}
						free(label);
					}
				}

			}

		}
		else
		{
		fprintf(output_file, "%s\n", zeros);
		line_counter++;
		}
		//memset(str_check, 0, MAX_STRLEN);
	}
	for (i = line_counter; i < final_line+1; i++)
	{
		if (words_table[i] != 0)
		{
			fprintf(output_file, "%05X\n", words_table[i] & 0xfffff);
		}
		else
		{
			fprintf(output_file, "%s\n", zeros);
		}
	}
	printf("%d\n", line_counter);
	printf("%d\n", final_line);
	free(str_check);
	fclose(asm_2nd_run);
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

	//scanf("%d", &dec);
	//printf("%X", (dec & 0xFFFFF));



	//str_check = fgets(str_check, 8, stdin);
	//is_hexa = is_hex(str_check);
	//printf("%d\n", is_hexa);
	//force_upper_hex(str_check);
	//printf("%s", str_check);
}