#include<stdio.h>
#include"../Service/Sale.h"
#include"Sale_UI.h"
#include"../Common/list.h"
#include"../Service/Play.h"
#include"Ticket_UI.h"
#include"../Service/Account.h"
#include"../Service/Ticket.h"
#include<time.h>
#include"../Service/Studio.h"
#include"../Service/Seat.h"
#include"../Persistence/EntityKey_Persit.h"
#include"../Persistence/Sale_Persist.h"
static const int SALE_PAGE_SIZE = 5;
static const int SALESANALYSIS_PAGE_SIZE = 5;
static const int TICKET_PAGE_SIZE = 10;
static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";
static const char SALE_KEY_NAME[] = "Sale";
#define PAGESIZE 5
extern account_t gl_CurUser;
int List_Foreach(ticket_list_t list,int id);
void Sale_UI_MgtEntry()
{
	system("cls");
	char choice; 
	char name[20];
	int i;
	int play_id;
	play_node_t *pos;


	play_list_t list;
	play_list_t list_fetch;
	List_Init(list,play_node_t);
	List_Init(list_fetch,play_node_t);

	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = SALE_PAGE_SIZE;
	Play_Srv_FetchAll(list_fetch);
	paging.totalRecords = Play_Srv_FetchAll(list);     //��ȡȫ�� ��Ŀ ��Ϣ
	Paging_Locate_FirstPage(list,paging);

	do
	{	printf(
				"\n\n");
		printf("��Ŀ�б� \n");
		printf("----------------------------------------------------------\n");

		printf( "%5s %15s %5s %10s %3s %3s %10s %10s     %3s\n","ID","��Ŀ����","��Ŀ����","��Ʒ����","�ȼ�","ʱ��","��ʼ����","��������","Ʊ��");
		printf( "--------------------------------------------------------------------------------------\n");

		Paging_ViewPage_ForEach(list,paging,play_node_t,pos,i)
		{
			printf( "%5d %15s %5d %10s %6d %6d %7d%3d%3d %4d%3d%3d     %4d\n",pos->data.id,pos->data.name,pos->data.type,pos->data.area,pos->data.rating,pos->data.duration,pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);	
		}
	

		printf( "----------------Total Recoeds: %2d---------------------------Page %2d   %2d   -----------\n",paging.totalRecords,Pageing_CurPage(paging),Pageing_TotalPages(paging));
	
		printf("----------------------------------\n");


		printf( "[C]��ĿID���Ҽƻ�      \n[S]��Ŀ���Ʋ��Ҽƻ�          \n[P]��һҳ       \n[N]��һҳ         \n[R]����          \n");
		printf( "----------");
		printf("���������ѡ��: ");
		setbuf(stdin,NULL);
		scanf( "%c",&choice);
		getchar( );
		switch(choice)
		{
			case'c':
			case'C':
				printf( "�������ĿID:");
				scanf( "%d",&play_id);
				getchar( );
				Sale_UI_ShowScheduler(play_id);
				break;
			case's':
			case'S':
				printf( "�������Ŀ����:");
				setbuf(stdin,NULL);
				gets(name);
				system("cls");
				Play_Srv_FetchByName(list_fetch,name);
				//Play_Srv_Print(list_fetch,name);
				break;

			case'n':
			case'N':
				if(!Pageing_IsLastPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
				}
				break;
			case'P':
			case'p':
				if(!Pageing_IsFirstPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging,-1, play_node_t);
				}
				break;
		}

	}while(choice != 'r' && choice != 'R');


}

void Sale_UI_ShowScheduler(int play_id)     //ͨ�� ��ĿID ��ʾ �ݳ��ƻ�
{
	system("cls");
	play_t buf;
	if (Play_Srv_FetchByID(play_id, &buf))
	{
		int listSize, sal_id, i;
		char choice;
		Pagination_t paging;
		schedule_list_t list, pos,p;
		schedule_t buf;
		List_Init(list, schedule_node_t);
		listSize = Schedule_Srv_FetchByPlay(list, play_id);
		paging.pageSize = PAGESIZE;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
	do
	{
		printf("                         �ƻ���                             \n");
			printf("%-3s  %-6s  %8s   %12s  %8s   %6s\n", "ID", "��ĿID", "�ݳ���ID", "��ӳ����", "��ӳʱ��", "��λ��");
			printf("--------------------------------------------------\n");
			//Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
			for (i = 0, pos = (schedule_node_t *)(paging.curPos); pos != list && i < paging.pageSize; i++, pos = pos->next)
			{  
				printf("%-3d     %-3d     %4d    %6d-%2d-%2d  %2d:%02d  %6d\n", pos->data.id, pos->data.play_id, pos->data.studio_id, pos->data.date.year, pos->data.date.month, pos->data.date.day, pos->data.time.hour, pos->data.time.minute,  pos->data.seat_count);
			}  
			printf("-----------------------------------------------------------\n");
			printf("-------�ܼ�¼��:%d ------------------- ҳ���� %d/%d -------\n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		
		printf( "[T]��ʾƱ     \n[N]��һ��     \n[P]��һ��    \n[R]���� \n");
		printf( "���ѡ��: ");

		scanf( "%c",&choice);
		getchar( );

		switch(choice)
		{
			case'T':
			case't':
				printf( "������ƻ�ID:");
				setbuf(stdin,NULL);
				int schedule_id;
				scanf("%d",&schedule_id);
				getchar( );
				Sale_UI_ShowTicket(schedule_id);
				break;
			case'N':
			case'n':
				if (!Pageing_IsLastPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
				}
				break;
			case'p':
			case'P':
				if (!Pageing_IsFirstPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
				}
				break;
	
		}
		

	}while(choice == 'r' && choice == 'R');
	}
	else
	{
		printf("Not Found !");
		getchar();
	}
}

int Sale_UI_ShowTicket(int schedule_id)
{
	system("cls");
	int pageSize = 10, i;
	int listSize;
	int ticket_id;
	char choice;
	ticket_list_t list, s;
	seat_t buf;
	Pagination_t paging;
	List_Init(list, ticket_node_t);
	listSize = Ticket_Srv_FetchBySchID_ticket(list,schedule_id);
	paging.pageSize = pageSize;
	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);
	do
	{
		printf("ӰƱ�б� :\n");
		printf("%-4s %10s   %-8s %4s %4s %8s\n", "id", "�ݳ��ƻ�id", "��λid", "��", "��", "Ʊ��״̬");
		printf("---------------------------------------------------\n");
		//for (i = 0, s = (ticket_node_t *)(paging.curPos); s != list && i < paging.pageSize; i++, s = s->next)
		Paging_ViewPage_ForEach(list, paging, ticket_node_t, s, i)
		{
			Seat_Srv_FetchByID(s->data.seat_id, &buf);
			if (buf.status == 1)
				printf("%-4d       %-6d   %-6d %4d %4d %8s\n", s->data.id, s->data.schedule_id, s->data.seat_id, buf.row , buf.column, Status(s->data.status));
		}
		printf("---------------------------------------------------\n");
		printf("========================�� %d �� ============================== �� %d/%d ҳ ================================\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("**    [R]������һ��     \n[B]��Ʊ��  \n[T]��Ʊ����   \n[P]��һҳ   ����\n[N]��һҳ��       \n");
		printf("Your Choice:");
		fflush(stdin);
		choice = getchar();
		switch (choice)
		{
		case 'B':
		case 'b':
			printf("\n��������Ҫ������Ʊ��id��");
			int ticket_id;
			scanf("%d",&ticket_id);
            getchar();
            Sale_UI_SellTicket(ticket_id);
			paging.totalRecords = Ticket_Srv_FetchBySchID_ticket(list,schedule_id);
			List_Paging(list, paging, ticket_node_t);
			break;
		case 'r':
		case 'R':
			break;
/*		case 't':
		case 'T':
			printf("\n��������Ҫ�˵�Ʊ��id��");
			scanf("%d", &ticket_id);
			getchar();
			//Sale_UI_ReturnTicket(ticket_id);
			paging.totalRecords = Ticket_Srv_FetchBySchID_ticket(list,schedule_id);
			List_Paging(list, paging, ticket_node_t);
			break;*/
		case 'p':
		case 'P':
			if(!Pageing_IsFirstPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, -1, ticket_node_t);
			}
			else
				printf("�޷���ת\n");
		case 'n':
		case 'N':
			if(!Pageing_IsLastPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, 1, ticket_node_t);
			}
			else
				printf("�޷���ת\n");
		}
	} while (choice != 'r' && choice != 'R');
}



/*int Sale_UI_SellTicket(ticket_list_t list_t,seat_list_t list_s)
{
	seat_node_t * seat = NULL;
	sale_t data_t;
	int row,col;
	while(1)
	{
		printf("����������Ҫ�������:");
		scanf( "%d",&row);
		printf("����������Ҫ�������:");
		scanf( "%d",&col);
		getchar( );
		seat = Seat_Srv_FindByRowCol(list_s,row,col);

		if(NULL == seat)
		{
			printf("��λ������!\n");
			continue;
		}

		if(seat->data.status == SEAT_BROKEN)
		{
			printf("��λ��!\n\n");
		}
		else if(seat->data.status == SEAT_NONE)
		{
			printf("��λΪ��!!\n");
		}
		else
		{
			break;
		}
	}
	
		ticket_t buf;
		account_t buf_a;
		Ticket_Srv_FetchByID(seat->data.id,&buf);
		if(buf.status == 1)
		{
			printf( "the ticket is Sale!\n");
			return 0;
		}
		else
		{
			data_t.ticket_id = buf.id;
			List_Foreach(list_t,buf.id);
			buf.status =TICKET_SOLD ;
			Ticket_Srv_Modify(&buf);
			while(1)
			{
				printf( "please input the saler ID:");
				scanf( "%d",&data_t.user_id);
				if(!Account_Srv_FetchByID(data_t.user_id,&buf_a))
				{
					continue;
				}
				else
				{
					break;
				}
			}
			data_t.type =SALE_SELL;
			data_t.value = buf.price;
			struct tm *p;
			time_t timep;
			time(&timep);
			p = localtime(&timep);
			data_t.date.year = p->tm_year + 1900;
	    	data_t.date.month = p->tm_mon + 1;
			data_t.date.day = p->tm_mday;
			data_t.time.hour = p->tm_hour;
			data_t.time.minute = p->tm_min;
			data_t.time.second = p->tm_sec;
			Sale_Srv_Add(&data_t);

			Ticket_UI_Print(buf);
		}
	
}
*/
int Sale_UI_SellTicket(int ticket_id)
{
	ticket_t buf, data;
	sale_t sale;

	if (Ticket_Srv_FetchByID(ticket_id, &buf))
	{
		if (buf.status == 1)
		{
			printf("��Ʊ���۳����س�����");
			getchar();
		}
		else
		{
			sale.id = EntKey_Perst_GetNewKeys(SALE_KEY_NAME,1);
			sale.user_id = gl_CurUser.id;
			sale.date = DateNow();
			sale.time = TimeNow();
			sale.ticket_id = buf.id;
			data.id = buf.id;
			data.schedule_id = buf.schedule_id;
			data.seat_id = buf.seat_id;
			sale.value=data.price = buf.price;
			data.status = TICKET_SOLD ;
			if (Ticket_Srv_Modify(&data) && Sale_Srv_Add(&sale))
			{
				printf("\t\t\t��������ʱ��:%d��-%02d��-%02d��   %dʱ-%02d��-%02d��\n", sale.date.year,sale.date.month,sale.date.day,sale.time.hour,sale.time.minute,sale.time.second);
				printf("\t\t\t��ƱԱID: %d\n",sale.user_id);
				Ticket_Info(&data);
				printf("\t\t\t�����ɹ����س�����");
				getchar();
			}
			else
			{
				printf("\t\t\t����ʧ�ܣ��س�����");
				getchar();
			}
		}
	}
	else
	{
		printf("�Ҳ�����Ʊ���س�����");
		getchar();
	}
    return 0;
}

/*oid Sale_UI_ReturnTicket()
{

    int id;
    ticket_t buf;
    sale_t refound;
    printf("������Ʊ��ID:");
    scanf("%d",&id);
    if(Ticket_Srv_FetchByticketID(id,&buf)==1)   //��ѯƱ�Ƿ����
    {
        if(buf.status == 1)//��ѯƱ�Ƿ��۳�
        {
	   buf.status =TICKET_AVL ; 
           Ticket_Srv_Modify(&buf);   //����Ticket_Srv_Modify�����޸�Ʊ��״̬
	   
	   refound.ticket_id = buf.id;
	   refound.value = buf.price;
	   refound.type = SALE_REFOUND;  //SALE_REFOUND��Ʊ
	   struct tm *p;
	   time_t timep;
	   time(&timep);
	   p = localtime(&timep);
	   refound.date.year = p->tm_year + 1900;
	   refound.date.month = p->tm_mon + 1;
	   refound.date.day = p->tm_mday;
	   refound.time.hour = p->tm_hour;
	   refound.time.minute = p->tm_min;
	   refound.time.second = p->tm_sec;
	   printf( "please input Salesperson ID:");
	   scanf( "%d",&refound.user_id);
	   getchar();
           Sale_Srv_Add(&refound);
        }
        else
        {
            printf("��Ʊ��δ���۳�");
        }
    }

    else
    {
        printf("Ʊ�����ݴ���");
    }

}
void Sale_UI_ReturnTicket(int ticket_id)
{
	ticket_t buf, data;
	sale_t a;
	if (Ticket_Srv_FetchByID(ticket_id, &buf))
	{
		if (buf.status == 0)
		{
			printf("\t\t\t��Ʊδ�۳����س�����");
			getchar();
		}
		else
		{
			//����Ʊ��id���Ʊ��Ʊ�Ļ�����Ϣ
			Select_Price_By_Playid(buf.id, &a);
			
			data.id = buf.id;
			data.schedule_id = buf.schedule_id;
			data.seat_id = buf.seat_id;
			data.price = buf.price;
			data.status = TICKET_AVL ;
			
			if (Ticket_Srv_Modify(&data)&& Sale_Srv_DeleteByID(a.id))
			{
				printf("\t\t\t�˶��ɹ����س�����");
				getchar();
			}
			else
			{
				printf("\t\t\t�˶�ʧ�ܣ��س�����");
				getchar();
			}
		}
	}
	else
	{
		printf("\t\t\t�Ҳ�����Ʊ���س�����");
		getchar();
	}
}*/
char * Status(int i)
{
	if (i == 1)
	{
		return "����";
	}
	else if (i == 0)
	{
		return "δ��";
	}
	else
	{
		return "����";
	}
}
int List_Foreach(ticket_list_t list,int id)
{
	while(1)
	{
		if(list->data.id == id)
		{
			list->data.status = TICKET_SOLD;
			break;
		}
		else
		{
			list = list->next;
		}
	}
}
void Ticket_Info(ticket_t *data)
{
	schedule_t p;
	play_t buf;
	seat_t s;
	studio_t studio;
	
	Seat_Srv_FetchByID(data->seat_id, &s);
	Schedule_Srv_FetchByID(data->schedule_id, &p);
	if (Play_Srv_FetchByID(p.play_id, &buf))
	{
		Studio_Srv_FetchByID(p.studio_id,&studio);
		printf("====================ӰƱ��Ϣ======================\n");
		printf("��Ŀ����: %s\n", buf.name);
		printf("���ͣ�%d\n", buf.type);
		printf("��Ʒ������%s\n", buf.area);
		printf("��Ŀ�ȼ���%d\n", buf.rating);
		printf("ʱ��: %d ����\n", buf.duration);
		printf("��ӳʱ��: %4d��%02d��%02d��\n",p.date.year,p.date.month,p.date.day);
		printf("��ӳʱ��: %2dʱ%02d��%02d��\n", p.time.hour, p.time.minute, p.time.second);
		printf("Ʊ�ۣ�%d Ԫ\n", buf.price);
		printf("�ݳ���:%s\n", studio.name);
		printf("λ��: %d��%d��\n", s.row + 1, s.column + 1);
		printf("======================================================\n");
		printf("Press Any Key To Continue......");
		getchar();
	}
	
void Sale_UI_Return()
{
	int ticket_id;
	int user_id;
	ticket_t *tt;
	sale_t refound;
	printf("������Ʊ��ID��");
	scanf("%d", &ticket_id);
	printf("��������ƱԱ��ID��");
	scanf("%d", &user_id);
	
	if(Ticket_Srv_FetchByticketID(ticket_id, tt) != 1)
	{
		printf("�޴�Ʊ\n");
		return ;
	}
	
	if(tt->status == 0)
	{
		printf("��Ʊδ�۳�\n");
		return ;
	}
	
	tt->status = 0; 
	Ticket_Srv_Modify(tt);
	
	refound.ticket_id = ticket_id;
	refound.user_id =user_id ;
	refound.date = DateNow();
	refound.time = TimeNow();
	refound.value = -tt->price;
	refound.type = -1;

	Sale_Srv_Add(&refound);	
}
}
