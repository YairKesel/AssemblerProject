
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#define MAX_STRLEN 500
#define MAX_LABEL 50
#define MAX_MEM 4096
//project definitions
char* op_codes_names[22][2] = { {"add","0"}, {"sub","1"}, {"mul","2"},{"and","3"},{"or","4"},{"xor","5"},{"sll","6"},{"sra","7"}, {"srl","8"}, {"beq","9"}, {"bne","10"}, {"blt","11"}, {"bgt","12"}, {"ble","13"}, {"bge","14"}, {"jal","15"},{"lw","16"}, {"sw","17"}, {"reti","18"}, {"in","19"}, {"out","20"}, {"halt","21"} };
char* reg_names[16][2] = { {"$zero","0"},{"$imm","1"},{"$v0","2"},{"$a0","3"},{"$a1","4"},{"$a2","5"},{"$a3","6"},{"$t0","7"},{"$t1","8"},{"$t2","9"},{"$s0","10"},{"$s1","11"},{"$s2","12"},{"$gp","13"},{"$sp","14"},{"$ra","15"} };
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
		printf("kaki");
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
			**comm_parts = *t;
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
int is_only_label(char *str)          //function to check if a line is only label (for readability in asmb code)
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
int is_i_type(char** array)										    //function the checks if a command is i type. if it does it returns the position of the string in the array.
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
void main(int argc, char* argv[])
{
	FILE* asmcode;
	FILE* output_file;
	int i = 0,j=0;
	int only_label=0, i_type_placement=0;												//flag for checking if a line is only a LABEL 
	char* str_check, * label, * no_label;
	char** command_parts;
	char** holder;
	char* wiped_str;
	char label_table[MAX_MEM][1];									//a table that will hold a label-line number relation
	str_check = (char*)malloc(MAX_STRLEN * sizeof(char));
	int line_countr;												// an intger that will follow the line number of the file (so we can place the line number of a label
																    // in the immidiate sector of other jump 
	/*
	output_file = fopen(argv[2],"w");
	asmcode = fopen(argv[1], "r");
	while (asmcode != NULL)
	{
		str_check=fgets(str_check, MAX_STRLEN, asmcode);
		if (str_check != NULL)
		{
			printf("%s\n", str_check);
			fprintf(output_file, "%s", str_check);
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
		printf("%s - %s\n", op_codes_names[i][0], op_codes_names[i][1]);
	}
	printf("\n\n\n");
	for (j = 0; j < 16; j++)
	{
		printf("%s - %s\n",reg_names[j][0], reg_names[j][1]);
	}
	*/

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
	//wiped_str = wipe_spaces(str_check);
	//printf("%s\n", wiped_str);
	//printf("test");
	label = Label_Identifier(str_check);
	if (label != NULL)
	{
		printf("%s\n", label);
		no_label = label_removal(str_check);
		//printf("This is no_label:\n");
		//printf("%s\n", no_label);
		//printf("test");
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
	i_type_placement = is_i_type(command_parts);
	printf("%d\n",i_type_placement);
	printf("%d", atoi(reg_names[4][1]));
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