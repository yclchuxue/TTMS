//ά���������Ͻ���

#include "MaiAccount_UI.h"

#include "../Common/List.h"
#include "../Service/Account.h"

extern account_t gl_CurUser;
#define len 100
#include <stdio.h>
#include <string.h>

void MaiAccount_UI_MgtEntry () {
    account_t data=gl_CurUser;
    char passwd[len],passwd2[len];
	int cnt = 0;
	getchar();
	int i;
	while(1)
    {
    	printf("\n����������:");
    	for(i= 0;i<len;i++)
		{
			setbuf(stdin,NULL);
			passwd[i] = getch();
			if(passwd[i] == '\n')
			{
				passwd[i] = '\0';
				break;
			}
			if((int)passwd[i] == 127)
			{
				printf("\b \b");
				i = i-2;
			}
			else
			{
				printf("*");
			}
			if(i < 0) passwd[0] = '\0';
		}
		printf("\n���ٴ���������:");
		for(i=0;i<len;i++)
		{
			setbuf(stdin,NULL);
			passwd2[i] = getch();
			if(passwd2[i] == '\n')
			{
				passwd2[i] = '\0';
				break;
			}
			if((int)passwd2[i] == 127)
			{
				printf("\b \b");
				i = i-2;
			}
			else
			{
				printf("*");
			}
			if(i < 0) passwd2[0] = '\0';
		}
		/*if(strcmp(passwd,passwd2) == 0)
		{
			break;
		}
		else
		{
		printf("\n�������벻һ��!"); 
		}*/
	}
    /*strcpy(data.password,passwd);
	encryption(data.password);

	printf("\n��������ܱ����룺");
	scanf("%d",&data.telephone);*/
    Account_Srv_Modify(&data);
	getchar();
	return ;
}
