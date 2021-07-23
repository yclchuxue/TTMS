#include"Ticket.h"
#include"../Common/list.h"
#include"Seat.h"
#include<stdlib.h>
#include"../Service/Schedule.h"
#include"../Persistence/Ticket_Persist.h"
#include<assert.h>
#include"Ticket.h"	
#include "Sale.h"

int  Ticket_Srv_GenBatch(int schedule_id)
{
	int count = 0;
	seat_list_t seat_head;

	List_Init(seat_head,seat_node_t);

	schedule_t buf;
	
	Schedule_Srv_FetchByID_ticket(schedule_id,&buf);
	count = Seat_Srv_FetchValidByRoomID(seat_head,buf.studio_id);
	Ticket_Perst_Insert(seat_head,schedule_id);

	return count;

}

int Ticket_Srv_DeleteBatch(int schedule_id)
{
	return Ticket_Perst_Rem(schedule_id);
}

int Ticket_Srv_FetchAll(ticket_list_t list)
{
	return Ticket_Perst_SelectAll(list);
}

int Ticket_Srv_FetchByticketID(int ticket_id,ticket_t *buf)
{
	return Ticket_Perst_SelByticketID(ticket_id,buf);
}
int Ticket_Srv_FetchByID(int id,ticket_t *buf)      
{
    return Ticket_Perst_SelByID(id,buf);
}

int Ticket_Srv_Modify(const ticket_t *data)
{
	return Ticket_Perst_Update(data);
}

int Ticket_Srv_FetchBySchID_ticket(ticket_list_t list,int id)
{
     return Ticket_Perst_SelBySchID(id,list);
}

/*
int Ticket_Srv_StatRevBySchID(int schedule_id, int *soldCount, int *totalCount){
	int value=0;
	ticket_list_t list, p;
	List_Init(list, ticket_node_t);

	*soldCount=0;
	*totalCount=Ticket_Srv_FetchBySchID_ticket(list, schedule_id);
	List_ForEach(list, p){
		if(p->data.status==TICKET_SOLD){
			(*soldCount)++	;
			value+=p->data.price;
		}
	}

	List_Destroy(list, ticket_node_t);
	return value;
}
*/

 //�����ݳ��ƻ�ID��ͳ�������ʼ�Ʊ��������Ʊ������
int Ticket_Srv_StatRevSchID(int schedule_id,int *soldCount)
{
	int value = 0;//���Ʊ��
	ticket_list_t list;
	ticket_node_t *p;
	sale_t sale;
	List_Init(list,ticket_node_t);
	*soldCount = 0;//��Ч��Ʊ������
	// *soldCount = Ticket_Srv_FetchBySchID(list,schedule_id);//+
	Ticket_Srv_FetchBySchID(list,schedule_id);
	List_ForEach(list,p)
	{
		Sale_Srv_FetchByTicketID(p->data.id,&sale);//+
		//sale.type = 1;
		if(sale.type == 1 && p->data.status == 1)//ȷ��Ϊ��Ʊ״̬����ȷ��Ϊ����
		{
			*soldCount++;
			value +=  p->data.price;
		}
	}

	List_Destroy(list,ticket_node_t);
	return value;
} 

int Schedule_Srv_FetchByID_ticket(int id,schedule_t *buf)
{
	return Schedule_Perst_SelectByID_ticket(id,buf);
}

int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id) {

	int count=0;

	//���ԭʼ����
	List_Free(list, ticket_node_t);

	//��ȡ�ݳ�Ʊ
	ticket_list_t tickList;
	List_Init(tickList, ticket_node_t);

	if(Ticket_Perst_SelectBySchID(tickList, schedule_id)<=0)	{
		List_Destroy(tickList, ticket_node_t);
		return 0;
	}

	List_Destroy(tickList, ticket_node_t);

	return count;
}
