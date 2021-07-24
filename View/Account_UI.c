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
		printf("�������Ժ\n");
		printf("[A]��¼\n");
		//printf("[B]ע��\n");
		printf("[C]�˳�\n");
		printf("���������ѡ��");

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
	printf("�������û�����");
	scanf("%s",name);

	int i = 0;

	while(1)
    {
		printf("����������:");
		while((ch=getch())!='\r')
		{
			passwd[j]=ch;
			printf("*");
			j=j+1;
			if(j>=30)
			{
				printf("\n\t�������,�����²�����\n");

				system("pause");
				//first_print(0); 
				memset(passwd, 0, sizeof(passwd));
			}
		}
	
		t=Account_Srv_Vertify(name,passwd);
		//printf("YYY%dYYY", t);
		//Account_Perst_SelByName(name,&test);
		if(t == 9)            //��ƱԱ 
		{
			printf("\n��½�ɹ���");
			system("pause");
			//Main_Menu_Sale();
			Main_Menu_Adinm();
			return 1;
		}
		else if(t == 2)            //���� 
		{
			printf("\n��½�ɹ���");
			system("pause");
			Main_Menu_manger();
			return 1;
		} 
		else if(t == 1)             //����Ա 
		{
			printf("\n��½�ɹ���");
			system("pause");
			//Main_Menu_Adinm();
			Main_Menu_Sale(); 
			return 1;
		}
		else	
		{
			printf("\n����������������룡");
			system("pause");
			i++;
		}
		if(i==3)
		{
			printf("������󳬹����Σ������µ�¼!");
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
		printf("********************** ϵͳ�û���Ϣ ****************************\n");
		printf("%5s   %8s   %10s   %10s\n", "ID", "����", "�û���","����");
		printf("----------------------------------------------------------------\n");
		
		//printf("BBBB\n"); 
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("%5d      %5d  %7s        %7s\n", pos->data.id,
					pos->data.type, pos->data.username, pos->data.password);
		}
		//printf("YYY\n");
		printf("-------------�ܼ�:%2d ----------- Page %2d/%2d -------------------\n",
				paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("***************************************************************\n");
		printf("[1]��һҳ\n");
		printf("[2]��һҳ\n");
		printf("[3]���ϵͳ�û�\n");
		printf("[4]�޸�ϵͳ�û�����\n");
		printf("[5]ɾ��ϵͳ�û�\n");
		printf("[6]��ѯϵͳ�û�\n");
		printf("[0]����\n");
		printf("\n===============================================================\n");
		
		fflush(stdin);
		printf("����������ѡ��");
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
			printf("������Ҫ�޸ĵ��û���:");
			scanf("%s", name);
			if (Account_UI_Modify(head,name)) 
			{	
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 5:
			system("cls");
			printf("������Ҫɾ�����û�����");
			scanf("%s", name);
			if (Account_UI_Delete(head,name)) 
			{	
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 6:
			system("cls");
			printf("������Ҫ��ѯ���û�����");
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
		printf("�˺Ų�����\n");
		system("pause");
		return 0;
	}
	char password[30];
	printf("���������룺");
	scanf("%s",password);
	strcpy(acc->data.password,password);
	if(Account_Srv_Modify(&(acc->data))==0)
	{
		printf("�޸�ʧ��\n");
		system("pause");
		return 0;
	}
	else
	{
		printf("�޸ĳɹ�\n");
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
	 	printf("%32s\n","������û�");
	 	printf("------------------------------------------------------------------\n");
		fflush(stdin);
	 	printf("[0]����       [1]����Ա       [2]����        [9]ϵͳ����Ա\n");
		printf("==================================================================\n");
	 	printf("��ѡ���˺����ͣ�");
	 	scanf("%d",&type);
		getchar();
	}while(type!=0 && type!=1 && type!=2 && type!=9);
 	buf->data.type = type;
 	//buf.id = EntKey_Srv_CompNewKey("Account");
 	printf("������Ҫ��ӵ��û���: ");
	scanf("%s",name);
	
	account_list_t acc;
	acc=(account_list_t)malloc(sizeof(account_node_t));
	acc=Account_Srv_FindByUsrName(list,name);
	if(acc!=NULL)
	{
		printf("���û��Ѵ���\n");
		system("pause");
		return 0; 
	}
	else
	{
		while(1)
		{
			printf("\n���������룺"); 
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
			printf("\n���ٴ��������룺"); 
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
				printf("���벻һ��");
				system("pause");
			}
		}
		strncpy(buf->data.password, ps1, sizeof(ps1));
		long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); //��ȡ����
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
 //�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	//printf("YYY\n");
	account_node_t *pos;
	pos=(account_list_t)malloc(sizeof(account_node_t));
	pos=Account_Srv_FindByUsrName(list,usrName);
	if(pos)
	{
		//printf("HHHHH\n");
		if(Account_Srv_DeleteByID(pos->data.id))
			printf("\tɾ���ɹ�\n");
		return 1;
	}
	printf("\t���˺Ų�����\n");
       	return  0;
}
//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t list,char usrName[])
{
	account_node_t *flag;
	flag=(account_list_t)malloc(sizeof(account_node_t));
	flag=Account_Srv_FindByUsrName(list,usrName);	
//	deciphering(flag->data.password);
	if(flag==NULL)
	{
		printf("���û������ڣ�");
		return 0;
	}
	else
	{
		printf("=============================\n");
		printf("id-------------%d\n",flag->data.id);
		printf("�û���----------%s\n",flag->data.username);
		printf("����------------%s\n",flag->data.password);
		printf("�û�����---------%d\n",flag->data.type);
		printf("\n\n");
	
	}
	return 1;

}
 
