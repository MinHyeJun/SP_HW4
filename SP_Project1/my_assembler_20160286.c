/*
 * ȭ�ϸ� : my_assembler_00000000.c 
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 * ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�. 
 *
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_20160286.h"

/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ���� 
 * ��ȯ : ���� = 0, ���� = < 0 
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�. 
 *		   ���� �߰������� �������� �ʴ´�. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[]) 
{
	if(init_my_assembler()< 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n"); 
		return -1 ; 
	}

	if(assem_pass1() < 0 ){
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n") ; 
		return -1 ; 
	}
	make_opcode_output("output_20160286.txt");

	/*
	* ���� ������Ʈ���� ���Ǵ� �κ�
	*
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n") ; 
		 return -1 ; 
	}

	make_objectcode_output("output") ; 
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���. 
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void)
{
	int result ; 

	if((result = init_inst_file("inst.data")) < 0 )
		return -1 ;
	if((result = init_input_file("input.txt")) < 0 )
		return -1 ; 
	return result ; 
}

/* ----------------------------------------------------------------------------------
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� 
 *        �����ϴ� �Լ��̴�. 
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
 *	
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */                                                      
int init_inst_file(char *inst_file)
{
	FILE * file;
	int errno;
	
	/* add your code here */
	char str[20];
	char *ptr;
	fopen_s(&file, inst_file, "r");

	if (file == NULL)
	{
		printf("������ ���� �� �����ϴ�.");
		errno =  -1;
	}
	else
	{

		while (0 == feof(file))
		{
			fgets(str, 20, file);

			ptr = strtok(str, " ");
			
			inst_table[inst_index] = malloc(sizeof(inst));
			strcpy(inst_table[inst_index]->inst, ptr);
			ptr = strtok(NULL, " ");
			inst_table[inst_index]->form = atoi(ptr);
			ptr = strtok(NULL, " ");
			strcpy(inst_table[inst_index]->opcode, ptr);
			ptr = strtok(NULL, " ");
			inst_table[inst_index]->oprnd_num = atoi(ptr);

			inst_index++;
		}

		errno = 0;
	}
	fclose(file);
	/*
	for(int j = 0; inst_table[j] != NULL; j++)
		printf("%s %d %s %d\n", inst_table[j]->inst, inst_table[j]->form, inst_table[j]->opcode, inst_table[j]->oprnd_num);
	*/
	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�. 
 * �Ű� : ������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0  
 * ���� : ���δ����� �����Ѵ�.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{
	FILE * file;
	int errno;

	/* add your code here */
	char str[20];
	char *ptr;
	fopen_s(&file, input_file, "r");

	if (file == NULL)
	{
		printf("������ ���� �� �����ϴ�.");
		errno = -1;
	}
	else
	{
		int  i = 0;

		while (0 == feof(file))
		{
			input_data[line_num] = malloc(sizeof(char) * 255);
			fgets(input_data[line_num], 255, file);

			line_num++;
		}

		errno = 0;
	}
	fclose(file);
	
	for(int j = 0; j < line_num; j++)
	 printf("%s", input_data[j]);
	
	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
 *        �н� 1�� ���� ȣ��ȴ�. 
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�  
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
	
	int op_index = -1;
	char * line, temp[100];

	if (str[0] == '.')
		return 0;

	char * input = malloc(sizeof(char) * 255);

	strcpy(input, str);

	line = strtok(input, "\t\n");
	token_table[token_line] = malloc(sizeof(token));
	token_table[token_line]->label = NULL;
	token_table[token_line]->operator = NULL;
	for (int i = 0; i < 3; i++)
		token_table[token_line]->operand[i] = NULL;
	token_table[token_line]->comment = NULL;
	
	if (str[0] != '\t')
	{
		token_table[token_line]->label = malloc(sizeof(char) * 20);
		strcpy(token_table[token_line]->label, line);
		line = strtok(NULL, "\t\n");
	}

	if (line != NULL)
	{
		op_index = search_opcode(line);
		token_table[token_line]->operator = malloc(sizeof(char) * 10);
		strcpy(token_table[token_line]->operator, line);
		line = strtok(NULL, "\t\n");
	}
	else
	{
		return 0;
	}

	if (line != NULL)
	{
		if (!(op_index >= 0 && inst_table[op_index]->oprnd_num == 0))
		{
			strcpy(temp, line);
			line = strtok(NULL, "\t\n");

			char * operand = strtok(temp, ",");

			for (int j = 0; operand != NULL; j++)
			{
				token_table[token_line]->operand[j] = malloc(sizeof(char) * 100);
				strcpy(token_table[token_line]->operand[j], operand);
				operand = strtok(NULL, ",");
			}
		}

		if (line != NULL)
		{
			token_table[token_line]->comment = malloc(sizeof(char) * 1024);
			strcpy(token_table[token_line]->comment, line);
		}
	}

	token_line++;
}

/* ----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
 * �Ű� : ��ū ������ ���е� ���ڿ� 
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
 * ���� : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str) 
{
	int i;
	char * inst;

	if (str[0] == '+')
		str = str + 1;

	for (i = 0; i < inst_index; i++)
	{
		if (strcmp(str, inst_table[i]->inst) == 0)
		{
			return i;
		}
	}

	return -1;
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	/* add your code here */

	/* input_data�� ���ڿ��� ���پ� �Է� �޾Ƽ� 
	 * token_parsing()�� ȣ���Ͽ� token_unit�� ����
	 */
	int i;
	for (i = 0; i < line_num; i++)
	{
		token_parsing(input_data[i]);
	}
	return 0;
}


/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 4��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char *file_name)
{
	/* add your code here */
	
	FILE * file;
	int op_index, input_index = 0, op_cnt;
	char lable[20], operator[8], operand[255], output[1023], init[1023] = { 0 };

	if (file_name == NULL)
	{
		printf("�Էµ� ���� �̸��� �����ϴ�.");
		return 0;
	}

	fopen_s(&file, file_name, "w");

	if (file == NULL)
	{
		printf("�ش� �̸��� ������ �ۼ��� �� �����ϴ�.");
		return 0;
	}
	
	for (int i = 0; i < token_line; i++)
	{
		strcpy(lable, init);
		strcpy(operator, init);
		strcpy(operand, init);
		strcpy(output, init);
		op_cnt = 0;

		op_index = search_opcode(token_table[i]->operator);

		if (token_table[i]->label != NULL)
			strcpy(lable, token_table[i]->label);

		strcpy(operator, token_table[i]->operator);
		
		for (int j = 0; token_table[i]->operand[j] != NULL; j++)
		{
			op_cnt++;
		}

		for (int j = 0; j < op_cnt; j++)
		{
			strcat(operand, token_table[i]->operand[j]);

			if (j < op_cnt - 1)
				strcat(operand, ",");
		}

		sprintf(output, "%-10s\t%s\t%-20s", lable, operator, operand);

		if (op_index < 0)
		{
			fprintf(file, "%s\n", output);
		}
		else
		{
			fprintf(file, "%s\t%s\n", output, inst_table[op_index]->opcode);
		}

		input_index++;
	}
	
	fclose(file);
}



/* --------------------------------------------------------------------------------*
* ------------------------- ���� ������Ʈ���� ����� �Լ� --------------------------*
* --------------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */

}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */

}