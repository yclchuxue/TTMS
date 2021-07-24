#include"Account_UI.h" 
#include "../Service/Account.h"
#include <stdio.h>
#include "../Common/list.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <conio.h>
extern account_t gl_CurUser;
static const char ACCOUNT_KEY_NAME[]="Account";

void Start()
{
	char choice;
	do
	{	
		system("cls");
		printf("河西村剧院\n");
		printf("[A]登录\n");
		//printf("[B]注册\n");
		printf("[C]退出\n");
		printf("请输入你的选择：");

		scanf("%c", &choice);
	
		if(choice == 'A' || choice == 'a')
		{
			SysLogin();
		}
		/*
		else if(choice == 'B' || choice == 'b')
		{
			account_list_t head;
			head=(account_list_t)malloc(sizeof(account_node_t));
			List_Init(head, account_node_t);
			Account_UI_Add(head);
		}
		*/
	}while(!(choice == 'C' || choice == 'c'));
}

int SysLogin()
{

	Account_Srv_InitSys();
	gl_CurUser.type=USR_ANOMY;
	char name[30],passwd[30];
	account_t test;

	int t,count=0,j=0;
	char ch;
	printf("请输入用户名：");
	scanf("%s",name);

	int i = 0;

	while(1)
    {
		printf("请输入密码:");
		while((ch=getch())!='\r')
		{
			passwd[j]=ch;
			printf("*");
			j=j+1;
			if(j>=30)
			{
				printf("\n\t密码过长,请重新操作！\n");

				system("pause");
				//first_print(0); 
				memset(passwd, 0, sizeof(passwd));
			}
		}
	
		t=Account_Srv_Vertify(name,passwd);
		//printf("YYY%dYYY", t);
		//Account_Perst_SelByName(name,&test);
		if(t == 9)            //售票员 
		{
			printf("\n登陆成功！");
			system("pause");
			//Main_Menu_Sale();
			Main_Menu_Adinm();
			return 1;
		}
		else if(t == 2)            //经理 
		{
			printf("\n登陆成功！");
			system("pause");
			Main_Menu_manger();
			return 1;
		} 
		else if(t == 1)             //管理员 
		{
			printf("\n登陆成功！");
			system("pause");
			//Main_Menu_Adinm();
			Main_Menu_Sale(); 
			return 1;
		}
		else	
		{
			printf("\n输入错误，请重新输入！");
			system("pause");
			i++;
		}
		if(i==3)
		{
			printf("输入错误超过三次，请重新登录!");
			system("pause");
			system("cls");
			return -1;
		}
	}
	return -1;
}
void Account_UI_MgtEntry()
{
	//printf("AAAA\n"); 
	account_list_t head;
	account_node_t *pos;
	head=(account_list_t)malloc(sizeof(account_node_t));
	pos=(account_node_t *)malloc(sizeof(account_node_t));
	int i;
	char name[100];
	

	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = 5;
	int choice;

	
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	system("cls");


	do {
		//printf("XXXX\n");
		printf("\n================================================================\n");
		printf("********************** 系统用户信息 ****************************\n");
		printf("%5s   %8s   %10s   %10s\n", "ID", "类型", "用户名","密码");
		printf("----------------------------------------------------------------\n");
		
		//printf("BBBB\n"); 
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("%5d      %5d  %7s        %7s\n", pos->data.id,
					pos->data.type, pos->data.username, pos->data.password);
		}
		//printf("YYY\n");
		printf("-------------总计:%2d ----------- Page %2d/%2d -------------------\n",
				paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("***************************************************************\n");
		printf("[1]上一页\n");
		printf("[2]下一页\n");
		printf("[3]添加系统用户\n");
		printf("[4]修改系统用户密码\n");
		printf("[5]删除系统用户\n");
		printf("[6]查询系统用户\n");
		printf("[0]返回\n");
		printf("\n===============================================================\n");
		
		fflush(stdin);
		printf("请输入您的选择：");
		fflush(stdin);
		scanf("%d", &choice);
	
		fflush(stdin);
		

		switch (choice) 
		{
		case 3:
			system("cls");
			if (Account_UI_Add(head)) 
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 4:
			system("cls");
			printf("输入你要修改的用户名:");
			scanf("%s", name);
			if (Account_UI_Modify(head,name)) 
			{	
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 5:
			system("cls");
			printf("请输入要删除的用户名：");
			scanf("%s", name);
			if (Account_UI_Delete(head,name)) 
			{	
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 6:
			system("cls");
			printf("请输入要查询的用户名：");
			scanf("%s",name);
			if(Account_UI_Query(head,name))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 1:
			system("cls");
			if (!Pageing_IsFirstPage(paging)) 
			{
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 2:
			system("cls");
			if (!Pageing_IsLastPage(paging)) 
			{
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 0);
	List_Destroy(head, account_node_t);

}

int Account_UI_Modify(account_list_t list,char userName[])
{
	account_list_t acc;
	acc=(account_list_t)malloc(sizeof(account_node_t));
	acc=Account_Srv_FindByUsrName(list, userName);
	if(acc==NULL)
	{
		printf("账号不存在\n");
		system("pause");
		return 0;
	}
	char password[30];
	printf("请输入密码：");
	scanf("%s",password);
	strcpy(acc->data.password,password);
	if(Account_Srv_Modify(&(acc->data))==0)
	{
		printf("修改失败\n");
		system("pause");
		return 0;
	}
	else
	{
		printf("修改成功\n");
		system("pause");
		return 1;
	}	
 } 
 int Account_UI_Add(account_list_t list)
 {
 	char ps1[30],ps2[30];
 	char name[30], ch;
 	int ans=0, j = 0;
 	int type;
 	account_node_t *buf;
 	buf=(account_list_t)malloc(sizeof(account_node_t));
	 
	//account_t data;
	 
	do 
	{
	 	printf("\n==================================================================\n");
	 	printf("%32s\n","添加新用户");
	 	printf("------------------------------------------------------------------\n");
		fflush(stdin);
	 	printf("[0]匿名       [1]销售员       [2]经理        [9]系统管理员\n");
		printf("==================================================================\n");
	 	printf("请选择账号类型：");
	 	scanf("%d",&type);
		getchar();
	}while(type!=0 && type!=1 && type!=2 && type!=9);
 	buf->data.type = type;
 	//buf.id = EntKey_Srv_CompNewKey("Account");
 	printf("请输入要添加的用户名: ");
	scanf("%s",name);
	
	account_list_t acc;
	acc=(account_list_t)malloc(sizeof(account_node_t));
	acc=Account_Srv_FindByUsrName(list,name);
	if(acc!=NULL)
	{
		printf("该用户已存在\n");
		system("pause");
		return 0; 
	}
	else
	{
		while(1)
		{
			printf("\n请输入密码："); 
			/*
			while((ch=getch()) != '\r')
			{
				ps1[j]=ch;
				j++;
				printf("*");
			}
			printf("%s\n", ps1);
			*/
			scanf("%s", ps1);
			fflush(stdin);
			printf("\n请再次输入密码："); 
			/*
			while((ch=getch())!='\r')
			{
				ps2[j]=ch;
				j++;
				printf("*");
			}
			printf("%s\n", ps2);
			*/
			scanf("%s",ps2);
			fflush(stdin);
			if(strcmp(ps1,ps2)==0)
			{
				break;
			}
			else
			{
				printf("输入不一致");
				system("pause");
			}
		}
		strncpy(buf->data.password, ps1, sizeof(ps1));
		long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); //获取主键
		if(key<=0)			
			return 0;
		buf->data.id = key;
		strcpy(buf->data.username,name);
	
		printf("****%d****\n", buf->data.type);
	
		Account_Srv_Add(&buf->data);
	
		
		List_AddTail(list,buf);
		return 1;
	}
 }
 //根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	//printf("YYY\n");
	account_node_t *pos;
	pos=(account_list_t)malloc(sizeof(account_node_t));
	pos=Account_Srv_FindByUsrName(list,usrName);
	if(pos)
	{
		//printf("HHHHH\n");
		if(Account_Srv_DeleteByID(pos->data.id))
			printf("\t删除成功\n");
		return 1;
	}
	printf("\t该账号不存在\n");
       	return  0;
}
//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t list,char usrName[])
{
	account_node_t *flag;
	flag=(account_list_t)malloc(sizeof(account_node_t));
	flag=Account_Srv_FindByUsrName(list,usrName);	
//	deciphering(flag->data.password);
	if(flag==NULL)
	{
		printf("该用户不存在！");
		return 0;
	}
	else
	{
		printf("=============================\n");
		printf("id-------------%d\n",flag->data.id);
		printf("用户名----------%s\n",flag->data.username);
		printf("密码------------%s\n",flag->data.password);
		printf("用户类型---------%d\n",flag->data.type);
		printf("\n\n");
	
	}
	return 1;

}
 
