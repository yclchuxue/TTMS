#ifndef TICKET_H_
#define TICKET_H_
#include"Schedule.h"

typedef enum
{
	TICKET_AVL = 0,   
	TICKET_SOLD = 1,    
	TICKET_RESV = 9    
}ticket_status_t;

typedef struct 
{
	int id;          
	int schedule_id;     
	int seat_id;      
	int price;        
	ticket_status_t status;      
}ticket_t;

typedef struct ticket_node
{
	ticket_t data;
	struct ticket_node *next,*prev;
}ticket_node_t,* ticket_list_t;



int Schedule_Srv_FetchByID_ticket(int id,schedule_t *buf);    

int Ticket_Srv_GenBatch(int schedule_id);       

int Ticket_Srv_deleteBatch(int schedule_id);    

int  Ticket_Srv_FetchAll(ticket_list_t list);

int Ticket_Srv_FetchByticketID(int ticket_id,ticket_t *buf);        

int Ticket_Srv_FetchBySchID_ticket(ticket_list_t list,int id);

int Ticket_Srv_FetchByID(int id,ticket_t *buf); 

int Ticket_Srv_StatRevSchID(int schedule_id,int *soldCount); 
//int Ticket_Srv_StatRevBySchID(int schedule_id, int *soldCount, int *totalCount);    

int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id);

#endif
