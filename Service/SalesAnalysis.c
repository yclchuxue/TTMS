//获取剧目票房
//剧目票房排行 

#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
//#include "../Persistence/Sale_Persist.h"
//#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"


//结合剧目Play.dat,统计销售数据，构建销售分析salesanalysis_list_t list链表，返回list链表长度
int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list)
{
	int cnt = 0;
	assert(NULL!=list);
	play_list_t playlist;
	play_node_t *pos;
	int sold;//存储已售有效票数
	salesanalysis_list_t p;
	salesanalysis_node_t newNode;
	List_Free(list,salesanalysis_node_t);
	List_Init(playlist,play_node_t);
	Play_Srv_FetchAll(playlist);


	List_ForEach(playlist,pos)
	{
		p  =   (salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t));
		strcpy(p->data.area,pos->data.area);
		p->data.duration = pos->data.duration;
		p->data.end_date = pos->data.end_date;
		strcpy(p->data.name,pos->data.name);
		p->data.play_id = pos->data.id;
		p->data.price = pos->data.price;
		p->data.start_date = pos->data.start_date;

		p->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id,&sold);//票房
		p->data.totaltickets = sold;//上座

		//List_AddTail(list,p);
		// printf("剧目id :%d\n",pos->data.id);
		cnt++;
	}

	List_Destroy(playlist,play_node_t);
	return cnt;

}


//对剧院电影票房排序
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list)
 { 
	salesanalysis_node_t *p;    
  	if(list == NULL) return ;   
    list->prev->next = NULL;//将循环断开     
	salesanalysis_list_t listleft =  list->next;//listleft指向第一个数据节点 
	list->next = list->prev = list;//将ｌｉｓｔ链表置为空  
	while(listleft != NULL)    
	{   
		       p = listleft;     
			    listleft = listleft->next;       
    SalesAnalysis_Srv_addnode(list,p);//+    
	}     
	return ; 
} 

void SalesAnalysis_Srv_addnode(salesanalysis_list_t list,salesanalysis_node_t *p)
{ 	
	salesanalysis_node_t *cur;   
    if(list == NULL)  
    {         
		List_AddTail(list,p);     
	}     
	else   
  	{        
	   cur = list->next;       
	     while(cur != list)      
		    {            
				 if(p->data.sales > cur->data.sales) break;  
				            cur = cur->next;      
		   }      
		      List_InsertBefore(cur,p);    
	}   
	  return ;
 }




