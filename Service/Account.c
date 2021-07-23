#include "Account.h"
#include "../View/Account_UI.h"
#include "../Common/List.h"
#include "../Persistence/Account_Persist.h"
#include "../Persistence/EntityKey_Persit.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static const char ACCOUNT_KEY_NAME[]="Account";
account_t gl_CurUser;

int Account_Srv_Add(const account_t *data)
{
	return Account_Perst_Insert(data);
}

int Account_Srv_Modify(const account_t *data)
{
	return Account_Perst_Update(data);
}

void Account_Srv_InitSys()
{
	if(Account_Perst_CheckAccFile())
	{
		return;
	}
	else
	{
		printf("��ʼ����\n");
		account_t data_admin;
		printf("���������������");
		scanf("%s",data_admin.username);
		long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); 
		if(key<=0)			
			return ;
		data_admin.id = key;



		printf("\n������������룺");
		scanf("%s",data_admin.password);
//		encryption(data_admin.password);
		printf("\n�����û����ͣ�1,2,9����");
		scanf("%d",&data_admin.type);

		Account_Srv_Add(&data_admin);
	}
	return;

}
int Account_Srv_Vertify(char usrName[], char pwd[])
{
	account_t usr;
	int t;
	t=Account_Perst_SelByName(usrName,&usr);
	if(t!=0)
	{
		gl_CurUser=usr;
		return t;
	}	
	return t;
}

//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���Null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {

	account_node_t *pos;
	int i;
	pos=(account_node_t *)malloc(sizeof(account_node_t));
	List_ForEach(list,pos)
	{
		//printf("A%sA\n", pos->data.username); 
		//int a = strlen(pos->data.username);
		//int b = strlen(usrName);
		if((i = strcmp(pos->data.username,usrName))==0)
		{
			return pos;
		}
		//printf("BBB%dBBB%d\t%d", i, a, b);
	}
	
	return NULL ;
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
int Account_Srv_DeleteByID(int usrID){
	int i;
	//printf("BBB\n");
	i=Account_Perst_DeleteByID(usrID);

	if(i==1)
	{
		printf("ɾ���û��ɹ���");
		return 1;
	}
	else
	{
		printf("ɾ��ʧ�ܣ�");
		return 0;
	}


}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
int Account_Srv_FetchAll(account_list_t list){
	int i;
	i=Account_Perst_SelectAll(list);
	return i;
}

int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName,buf);
}
