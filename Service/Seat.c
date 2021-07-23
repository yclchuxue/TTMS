#include <stdlib.h>
#include "../Common/List.h"
#include "Seat.h"
#include <stdio.h>
#include "../Persistence/Seat_Persist.h"

/*
函数功能：用于添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_Srv_Add(const seat_t *data){
	return Seat_Perst_Insert(data);
}

/*
函数功能：批量添加座位数据。
参数说明：list为seat_list_t类型指针，表示需要添加的批量座位数据形成的链表。
返 回 值：整型，表示是否成功添加了批量座位的标志。
*/
int Seat_Srv_AddBatch(seat_list_t list){
	return Seat_Perst_InsertBatch(list);
}

/*
函数功能：用于修改一个座位数据。
参数说明：data为seat_t类型指针，表示需要修改的座位数据结点。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_Srv_Modify(const seat_t *data){
	return Seat_Perst_Update(data);
}

/*
函数功能：根据座位ID删除一个座位。
参数说明：ID为整型，表示需要删除的座位数据结点。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Srv_DeleteByID(int ID){
	 return Seat_Perst_DeleteByID(ID);
}

/*
函数功能：根据座位ID获取座位数据。
参数说明：第一个参数ID为整型，表示座位ID，第二个参数buf为seat_t指针，指向待获取的座位数据结点。
返 回 值：整型，表示是否成功获取了座位的标志。
*/
int Seat_Srv_FetchByID(int ID, seat_t *buf){
       return Seat_Perst_SelectByID(ID, buf);
}

/*
函数功能：根据演出厅ID删除所有座位。
参数说明：roomID为整型，表示需要删除所有座位的演出厅ID。
返 回 值：整型，表示是否成功删除了演出厅所有座位的标志。
*/
inline int Seat_Srv_DeleteAllByRoomID(int roomID){
	return Seat_Perst_DeleteAllByRoomID(roomID);
}

/*
函数功能：根据给定演出厅的行、列数初始化演出厅的所有座位数据，并将每个座位结点按行插入座位链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功初始化了演出厅的所有座位。
*/
int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID){
	
    int SeatCount=Seat_Perst_SelectByRoomID(list, roomID);
	Seat_Srv_SortSeatList(list);
	return SeatCount;
}

/*
函数功能：根据演出厅ID获得该演出厅的有效座位。
参数说明：第一个参数list为seat_list_t类型，表示获取到的有效座位链表头指针，第二个参数roomID为整型，表示需要提取有效座位的演出厅ID。
返 回 值：整型，表示演出厅的有效座位个数。
*/
int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID)
{
    int SeatCount;
    seat_node_t *node;
    
    SeatCount=Seat_Perst_SelectByRoomID(list, roomID);
    List_ForEach(list, node)
	{
		if(node->data.status!=SEAT_GOOD)
		{
			List_FreeNode(node);
			SeatCount--;
		}
	}	
	Seat_Srv_SortSeatList(list);
	
	return SeatCount;
}

/*
函数功能：根据给定演出厅的行、列数初始化演出厅的所有座位数据，并将每个座位结点按行插入座位链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功初始化了演出厅的所有座位。
*/
int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount,
		int colsCount) {
		int i, j;
		seat_node_t *node;
		
		for(i = 1; i < rowsCount+1; i++)
		{
			for(j = 1; j < colsCount+1; j++)
			{
				node = (seat_node_t *)malloc(sizeof(seat_node_t));
				node->data.roomID = roomID;
				node->data.row = i;
				node->data.column = j;
				node->data.status = SEAT_GOOD;
				node->data.id = EntKey_Perst_GetNewKeys("Seat", 1); 
			    if(node->data.id == 0)
			    {
			        printf("获取主键失败\n");
			        return -1;
			    }
				List_AddTail(list, node);
			}
		}
       return Seat_Perst_InsertBatch(list);
}

/*
函数功能：对座位链表list按座位行号、列号进行排序。
参数说明：list为seat_list_t类型，表示待排序座位链表头指针。
返 回 值：无。
*/
void Seat_Srv_SortSeatList(seat_list_t list) {
    
	seat_node_t *p, *q;
	
	assert(list!=NULL);

	if(List_IsEmpty(list))
		return ;

	//将next指针构成的循环链表从最后一个结点断开
	list->prev->next = NULL;

	//q指向第一个数据节点
	q = list->next;

	//将list链表置为空
	list->next = list->prev = list;

	while (q != NULL ) {
		//取出第一个结点
		p = q;
		q = q->next;

		//将结点p加入到已排序链表list中
		Seat_Srv_AddToSoftedList(list, p);
	}
}

/*
函数功能：将一个座位结点加入到已排序的座位链表中。
参数说明：第一个参数list为seat_list_t类型，表示待插入结点的座位链表头指针，第二个参数node为seat_node_t指针，表示需要插入的座位数据结点。
返 回 值：无。
*/
void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node) {
    seat_node_t *p;

	assert(list!=NULL && node!=NULL);

	if(List_IsEmpty(list))	{
		List_AddTail(list, node);
	}else{
		//寻找插入位置
		p=list->next;

		while(p!=list && (p->data.row<node->data.row ||
				(p->data.row==node->data.row && p->data.column<node->data.column))){
			p=p->next;
		}

		//将结点node加入到p之前
		List_InsertBefore(p, node);
	}
}

/*
函数功能：根据座位的行、列号获取座位数据。
参数说明：第一个参数list为seat_list_t类型，表示座位链表头指针，
         第二个参数row为整型，表示待获取座位的行号，第三个参数column为整型，表示待获取座位的列号。
返 回 值：为seat_node_t指针，表示获取到的座位数据。
*/
seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
    
	seat_node_t *node;

	List_ForEach(list,node)
		if ((node->data.row == row) && (node->data.column == column))
			return node;
			
	return NULL ;
}

/*
函数功能：根据座位ID在链表中获取座位数据。
参数说明：第一个参数list为seat_list_t类型，指向座位数据链表，第二个参数ID为整型，表示座位ID。
返 回 值：seat_node_t类型，表示获取的座位数据。
*/
seat_node_t * Seat_Srv_FindByID(seat_list_t list, int rowID) {
       // 请补充完整
       return NULL;
}
