
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#define MAX_STRLEN 500
#define MAX_LABEL 50
#define MAX_MEM 4096
//project definitions
char* op_codes_names[] = { "add","sub","mul","and","or","xor","sll","sra","srl","beq","bne","blt","bgt","ble","bge","jal","lw","sw","reti","in","out","halt" };
char* reg_names[] = { "$zero","$imm","$v0","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$s0","$s1","$s2","$gp","$sp","$ra" };
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
void main(int argc, char* argv[])
{
	int i = 0;
	int only_label=0;							//flag for checking if a line is only a LABEL 
	char* str_check, * label, * no_label;
	char** command_parts;
	char* wiped_str;
	char label_table[MAX_MEM][1]; //a table that will hold a label-line number relation
	str_check = (char*)malloc(MAX_STRLEN * sizeof(char));
	int line_countr; // an intger that will follow the line number of the file (so we can place the line number of a label
					 // in the immidiate sector of other jump 
	fgets(str_check, (MAX_STRLEN) * sizeof(char), stdin);
	//printf("%s", str_check);
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
	
	for (i = 0; i < 5; i++)						// test loop for contents of the array 
	{
		//printf("here\n");
		printf("%s\n", *command_parts);
		command_parts++;
		//printf("found psik\n");
	}
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