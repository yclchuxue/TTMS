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
	paging.totalRecords = Play_Srv_FetchAll(list);     //获取全部 剧目 信息
	Paging_Locate_FirstPage(list,paging);

	do
	{	printf(
				"\n\n");
		printf("剧目列表 \n");
		printf("----------------------------------------------------------\n");

		printf( "%5s %15s %5s %10s %3s %3s %10s %10s     %3s\n","ID","剧目名称","剧目类型","出品地区","等级","时长","开始日期","结束日期","票价");
		printf( "--------------------------------------------------------------------------------------\n");

		Paging_ViewPage_ForEach(list,paging,play_node_t,pos,i)
		{
			printf( "%5d %15s %5d %10s %6d %6d %7d%3d%3d %4d%3d%3d     %4d\n",pos->data.id,pos->data.name,pos->data.type,pos->data.area,pos->data.rating,pos->data.duration,pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);	
		}
	

		printf( "----------------Total Recoeds: %2d---------------------------Page %2d   %2d   -----------\n",paging.totalRecords,Pageing_CurPage(paging),Pageing_TotalPages(paging));
	
		printf("----------------------------------\n");


		printf( "[C]剧目ID查找计划      \n[S]剧目名称查找计划          \n[P]上一页       \n[N]下一页         \n[R]返回          \n");
		printf( "----------");
		printf("请输入你的选择: ");
		setbuf(stdin,NULL);
		scanf( "%c",&choice);
		getchar( );
		switch(choice)
		{
			case'c':
			case'C':
				printf( "请输入剧目ID:");
				scanf( "%d",&play_id);
				getchar( );
				Sale_UI_ShowScheduler(play_id);
				break;
			case's':
			case'S':
				printf( "请输入剧目名称:");
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

void Sale_UI_ShowScheduler(int play_id)     //通过 剧目ID 显示 演出计划
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
		printf("                         计划表                             \n");
			printf("%-3s  %-6s  %8s   %12s  %8s   %6s\n", "ID", "剧目ID", "演出厅ID", "放映日期", "放映时间", "座位数");
			printf("--------------------------------------------------\n");
			//Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
			for (i = 0, pos = (schedule_node_t *)(paging.curPos); pos != list && i < paging.pageSize; i++, pos = pos->next)
			{  
				printf("%-3d     %-3d     %4d    %6d-%2d-%2d  %2d:%02d  %6d\n", pos->data.id, pos->data.play_id, pos->data.studio_id, pos->data.date.year, pos->data.date.month, pos->data.date.day, pos->data.time.hour, pos->data.time.minute,  pos->data.seat_count);
			}  
			printf("-----------------------------------------------------------\n");
			printf("-------总记录数:%d ------------------- 页数： %d/%d -------\n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		
		printf( "[T]显示票     \n[N]下一个     \n[P]上一个    \n[R]返回 \n");
		printf( "你的选择: ");

		scanf( "%c",&choice);
		getchar( );

		switch(choice)
		{
			case'T':
			case't':
				printf( "请输入计划ID:");
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
		printf("影票列表 :\n");
		printf("%-4s %10s   %-8s %4s %4s %8s\n", "id", "演出计划id", "座位id", "行", "列", "票的状态");
		printf("---------------------------------------------------\n");
		//for (i = 0, s = (ticket_node_t *)(paging.curPos); s != list && i < paging.pageSize; i++, s = s->next)
		Paging_ViewPage_ForEach(list, paging, ticket_node_t, s, i)
		{
			Seat_Srv_FetchByID(s->data.seat_id, &buf);
			if (buf.status == 1)
				printf("%-4d       %-6d   %-6d %4d %4d %8s\n", s->data.id, s->data.schedule_id, s->data.seat_id, buf.row , buf.column, Status(s->data.status));
		}
		printf("---------------------------------------------------\n");
		printf("========================共 %d 项 ============================== 第 %d/%d 页 ================================\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("**    [R]返回上一级     \n[B]订票　  \n[T]退票　　   \n[P]上一页   　　\n[N]下一页　       \n");
		printf("Your Choice:");
		fflush(stdin);
		choice = getchar();
		switch (choice)
		{
		case 'B':
		case 'b':
			printf("\n请输入你要订购的票的id：");
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
			printf("\n请输入你要退的票的id：");
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
				printf("无法跳转\n");
		case 'n':
		case 'N':
			if(!Pageing_IsLastPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, 1, ticket_node_t);
			}
			else
				printf("无法跳转\n");
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
		printf("请输入您想要买的行数:");
		scanf( "%d",&row);
		printf("请输入您想要买的列数:");
		scanf( "%d",&col);
		getchar( );
		seat = Seat_Srv_FindByRowCol(list_s,row,col);

		if(NULL == seat)
		{
			printf("座位不存在!\n");
			continue;
		}

		if(seat->data.status == SEAT_BROKEN)
		{
			printf("座位损坏!\n\n");
		}
		else if(seat->data.status == SEAT_NONE)
		{
			printf("座位为空!!\n");
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
			printf("此票已售出！回车继续");
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
				printf("\t\t\t订单生成时间:%d年-%02d月-%02d日   %d时-%02d分-%02d秒\n", sale.date.year,sale.date.month,sale.date.day,sale.time.hour,sale.time.minute,sale.time.second);
				printf("\t\t\t售票员ID: %d\n",sale.user_id);
				Ticket_Info(&data);
				printf("\t\t\t订购成功！回车继续");
				getchar();
			}
			else
			{
				printf("\t\t\t订购失败！回车继续");
				getchar();
			}
		}
	}
	else
	{
		printf("找不到此票！回车继续");
		getchar();
	}
    return 0;
}

/*oid Sale_UI_ReturnTicket()
{

    int id;
    ticket_t buf;
    sale_t refound;
    printf("请输入票的ID:");
    scanf("%d",&id);
    if(Ticket_Srv_FetchByticketID(id,&buf)==1)   //查询票是否存在
    {
        if(buf.status == 1)//查询票是否售出
        {
	   buf.status =TICKET_AVL ; 
           Ticket_Srv_Modify(&buf);   //调用Ticket_Srv_Modify函数修改票的状态
	   
	   refound.ticket_id = buf.id;
	   refound.value = buf.price;
	   refound.type = SALE_REFOUND;  //SALE_REFOUND退票
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
            printf("该票还未被售出");
        }
    }

    else
    {
        printf("票务数据错误");
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
			printf("\t\t\t此票未售出！回车继续");
			getchar();
		}
		else
		{
			//根据票的id获得票售票的基本信息
			Select_Price_By_Playid(buf.id, &a);
			
			data.id = buf.id;
			data.schedule_id = buf.schedule_id;
			data.seat_id = buf.seat_id;
			data.price = buf.price;
			data.status = TICKET_AVL ;
			
			if (Ticket_Srv_Modify(&data)&& Sale_Srv_DeleteByID(a.id))
			{
				printf("\t\t\t退订成功！回车继续");
				getchar();
			}
			else
			{
				printf("\t\t\t退订失败！回车继续");
				getchar();
			}
		}
	}
	else
	{
		printf("\t\t\t找不到此票！回车继续");
		getchar();
	}
}*/
char * Status(int i)
{
	if (i == 1)
	{
		return "已售";
	}
	else if (i == 0)
	{
		return "未售";
	}
	else
	{
		return "其他";
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
		printf("====================影票信息======================\n");
		printf("剧目名称: %s\n", buf.name);
		printf("类型：%d\n", buf.type);
		printf("出品地区：%s\n", buf.area);
		printf("剧目等级：%d\n", buf.rating);
		printf("时长: %d 分钟\n", buf.duration);
		printf("放映时间: %4d年%02d月%02d日\n",p.date.year,p.date.month,p.date.day);
		printf("放映时间: %2d时%02d分%02d秒\n", p.time.hour, p.time.minute, p.time.second);
		printf("票价：%d 元\n", buf.price);
		printf("演出厅:%s\n", studio.name);
		printf("位置: %d行%d列\n", s.row + 1, s.column + 1);
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
	printf("请输入票的ID：");
	scanf("%d", &ticket_id);
	printf("请输入售票员的ID：");
	scanf("%d", &user_id);
	
	if(Ticket_Srv_FetchByticketID(ticket_id, tt) != 1)
	{
		printf("无此票\n");
		return ;
	}
	
	if(tt->status == 0)
	{
		printf("该票未售出\n");
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
