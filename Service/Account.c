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
		printf("初始化：\n");
		account_t data_admin;
		printf("请输入你的姓名：");
		scanf("%s",data_admin.username);
		long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); 
		if(key<=0)			
			return ;
		data_admin.id = key;



		printf("\n请输入你的密码：");
		scanf("%s",data_admin.password);
//		encryption(data_admin.password);
		printf("\n输入用户类型（1,2,9）：");
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

//验证用户名是否存在，遍历list，若存在，返回对应结点指针；否则，返回Null
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

//删除一个用户账号，通过调用Account_Perst_DeleteByID(usrID)函数实现
int Account_Srv_DeleteByID(int usrID){
	int i;
	//printf("BBB\n");
	i=Account_Perst_DeleteByID(usrID);

	if(i==1)
	{
		printf("删除用户成功！");
		return 1;
	}
	else
	{
		printf("删除失败！");
		return 0;
	}


}

//提取所有用户账号信息，保存到list链表中，通过调用Account_Perst_SelectAll(list)函数实现
int Account_Srv_FetchAll(account_list_t list){
	int i;
	i=Account_Perst_SelectAll(list);
	return i;
}

int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName,buf);
}
