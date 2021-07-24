#include"../View/Ticket_UI.h"
#include"../Common/list.h"
#include"../Service/Ticket.h" 
#include"../Service/Schedule.h"
#include "../Service/Account.h"
#include"../Service/Schedule.h"
#include"../Service/Play.h"
static const int TICKET_PAGE_SIZE = 5;
#include "Play_UI.h"
#include<stdio.h>


void Ticket_UI_MgtEntry(int schedule_id)   
{   
	int i,id;
	int yan_id;
	char choice;
	schedule_t buf;
	play_t data;
	Schedule_Srv_FetchByID_ticket(schedule_id,&buf);
	Play_Srv_FetchByID(buf.play_id,&data);

	printf( "=======================================================================\n");
	
	printf( "剧目名称          演出厅ID                日期                  时间       \n");
	printf( "%s                 %d             %d.%d.%d               %d:%d:%d\n",data.name,buf.studio_id,buf.date.year,buf.date.month,buf.date.day,buf.time.hour,buf.time.minute,buf.time.second);

	printf( "[G]生成票    [R]删除票 \n");
	setbuf(stdin,NULL);
	printf("请输入你的选择:");
	scanf("%c",&choice);
	getchar( );

	switch(choice)
	{
		case'G':
		case'g':
			system("cls");
			Ticket_Srv_GenBatch(schedule_id);
			system("pause"); 
			printf("生成成功!\n");
			break;
		case'r':
		case'R':
			Ticket_Srv_DeleteBatch(schedule_id);
			Ticket_Srv_GenBatch(schedule_id);
			printf("删除成功!\n");
			break;
		case'q':
		case'Q':
			Ticket_UI_Query();
			break;
	}

}

void Ticket_UI_Query(void)
{

	printf( "==================================================\n");
	printf( "*********************查找票***********************\n");
	printf( "请输入票的ID:");

	int id;
	scanf("%d",&id);
	getchar( );

	Ticket_UI_ShowTicket(id);
}


int Ticket_UI_ShowTicket(int ticket_id)
{
	ticket_t buf;
	int temp = Ticket_Srv_FetchByticketID(ticket_id,&buf);
	if(temp == 0)
	{
		printf( "该票不存在!\n");
	}
	else
	{
		printf( "演出票ID      计划ID      座位ID     票价      票态    \n");
		printf( "%6d  %6d     %6d    %6d   %6d    \n",buf.id,buf.schedule_id,buf.seat_id,buf.price,buf.status);
	}
}

void Ticket_UI_Print(const ticket_t data)
{
	printf("该票的信息为 : \n");
	printf( "演出票ID      计划ID    座位ID  票价\n");
	printf( "%d                  %d           %d      %d    \n",data.id,data.schedule_id,data.seat_id,data.price);
	printf( "---------------------------------------\n");
	getchar( );
}
