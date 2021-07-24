#include "Seat_UI.h"
#include "../Service/Seat.h"

#include "../Service/Studio.h"
#include "../Common/List.h"
#include <stdio.h>

/*
表识符：TTMS_SCU_Seat_UI_S2C 
函数功能：根据座位状态获取界面显示符号。
参数说明：status为seat_status_t类型，表示座位状态。
返 回 值：字符型，表示座位的界面显示符号。
*/
inline char Seat_UI_Status2Char(seat_status_t status) {
	
	char statusChar;
	
	switch (status) 
	{
		case SEAT_GOOD:	
			statusChar = '#';
			break;
		case SEAT_BROKEN:
			statusChar = '~';
			break;
		case SEAT_NONE:
		default:
			statusChar = ' ';
			break;
	}
	
	return statusChar;
}

/*
标识符：TTMS_SCU_Seat_UI_C2S
函数功能：根据输入符号获取座位状态。
参数说明：statusChar为字符型，表示设置座位的输入符号。
返 回 值：seat_status_t类型，表示座位的状态。
*/
inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	
	seat_status_t status;
	
	switch (statusChar) 
	{
		case '#':	
			status = SEAT_GOOD;
			break;
		case '~':	
			status = SEAT_BROKEN;
			break;
		default:
			status = SEAT_NONE;
			break;
	}
	
	return status;
}

/*
标识符：TTMS_SCU_Seat_UI_MgtEnt
函数功能：界面层管理座位的入口函数，显示当前的座位数据，并提供座位数据添加、修改、删除功能操作的入口。
参数说明：roomID为整型，是需要设置座位的演出厅ID。
返 回 值：无。
*/ 
void Seat_UI_MgtEntry(int roomID) {
	
	studio_t stu;
	seat_list_t list;
	seat_node_t *node;
	seat_node_t *p;
	char choice;
	int changedCount, i, j;
	
	List_Init(list, seat_node_t);
	
	if(!Studio_Srv_FetchByID(roomID, &stu))
	{
		printf("演出厅不存在\n");
		return ;
	}
	
	if(!Seat_Srv_FetchByRoomID(list, roomID))
	{
		stu.seatsCount = Seat_Srv_RoomInit(list, roomID, stu.rowsCount, stu.colsCount);
		Studio_Srv_Modify(&stu);
	}
	
	while(1)
	{
		system("cls");
		printf("======================================================================\n");
		printf("===============================座位表=================================\n");
		for(i = 1; i < stu.rowsCount+1; i++)
		{
			j = 1;
			printf("\t");
			List_ForEach(list, node)
			{
				if (node->data.row == i) {
					while (node->data.column != j) {
						printf(" ");
						j++;
					}
					printf("%c", Seat_UI_Status2Char(node->data.status));
					j++;
				}
			}
			printf("\n");	
		}
		printf("======================================================================\n");
		printf("----------------------------\n");
		printf("1.添加座位(a/A)\n");
		printf("2.修改座位(u/U)\n");
		printf("3.删除座位(d/D)\n");
		printf("4.返回上一级(r/R)\n");
		printf("----------------------------\n");
		printf("输入相应字母："); 
		fflush(stdin);
		choice = getchar();
		
		if((choice == 'a') || (choice == 'A'))
		{
			changedCount = Seat_UI_Add(list, roomID, stu.rowsCount, stu.colsCount);
			if(changedCount > 0)
			{
				stu.seatsCount += changedCount;
				Studio_Srv_Modify(&stu);
			}
		}else if((choice == 'u') || (choice == 'U'))
		{
			Seat_UI_Modify(list, stu.rowsCount, stu.colsCount);
		}else if((choice == 'd') || (choice == 'D'))
		{
			changedCount = Seat_UI_Delete(list, stu.rowsCount, stu.colsCount);
			if(changedCount > 0)
			{
				stu.seatsCount -= changedCount;
				Studio_Srv_Modify(&stu);
			}
		}else if((choice == 'r') || (choice == 'R'))
			return ;
	}
}

/*
识符：TTMS_SCU_Seat_UI_Add
函数功能：用于添加一个新的座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，
         第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //输入一个座位
	char choice;
	int newRecCount = 0;
	seat_node_t *p;
	seat_t rec;

	while(1)
	{
		while(1)
		{
			printf("输入行号：");
			scanf("%d", &rec.row);
			printf("输入列号：");
			scanf("%d", &rec.column);
			if(rec.row <= row && rec.column <= column)       //满足要求
				break;
			else
				printf("请输入正确序号\n");
		}

		p = Seat_Srv_FindByRowCol(list, rec.row, rec.column);   //判断座位是否已经存在
		if(p)
			printf("该座位已经存在\n");
		else
		{
			rec.id = EntKey_Perst_GetNewKeys("Seat", 1);	
			rec.roomID = roomID;
			rec.status = SEAT_GOOD;    
			if(Seat_Srv_Add(&rec))
			{
				newRecCount++;
				printf("添加成功\n");
				p = (seat_node_t *)malloc(sizeof(seat_node_t));
				p->data = rec;
				Seat_Srv_AddToSoftedList(list, p);
			}
			else
				printf("新座位添加失败！\n");
		}
		
		printf("-------------------------------------------\n");
		printf("1.继续添加(a/A)\n2.返回(r/R)\n");
		printf("输入你的选择:");
		fflush(stdin);
		scanf("%c", &choice);
		if(choice == 'r' || choice == 'R')
		{
			break;
		}
	}
	return newRecCount;
}

/*
标识符：TTMS_SCU_Seat_UI_Mod 
函数功能：用于修改一个座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_UI_Modify(seat_list_t list, int row, int column) {
	
	int ret = 0;
	int newrow, newcolumn;
	char choice;
	seat_node_t *p;

	while (1)
	{
		while(1)
		{
			printf("请输入行号：");
			scanf("%d", &newrow);
			printf("请输入列号: ");
			scanf("%d", &newcolumn);
			if(newrow <= row && newcolumn <= column)
				break;
			else
				printf("座位不存在\n");
		}

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn); 
		if(p)
		{
			printf("-----------------\n");
			printf(" :空座位\n");
			printf("~:坏座位\n");
			printf("#:可选择座位\n"); 
			printf("-----------------\n");
			printf("选择座位状态:");
			fflush(stdin);
			p->data.status = Seat_UI_Char2Status(getchar());                         
			if (Seat_Srv_Modify(&(p->data))) 
			{
				ret = 1;
				printf("座位更新成功\n");
			} 
			else
				printf("座位更新失败\n");
		}
		else
		{
			printf("座位不存在\n");
		}
		printf("-----------------------------------\n");
		printf("1.继续添加(u/U)\n2.返回(r/R)\n");
		fflush(stdin);
		scanf("%c", &choice);
		if(choice == 'r' || choice == 'R')
		{
			break;
		}
	}
	
	return ret;
}

/*
标识符：TTMS_SCU_Seat_UI_Del
函数功能：用于删除一个座位的数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_UI_Delete(seat_list_t list, int row, int column) {
	
	int ret = 0;
	int newrow, newcolumn;
	seat_node_t *p;
	char choice;

	do 
	{
		do 
		{
			printf("座位行号： ");
			scanf("%d", &(newrow));
			printf("座位列号： ");
			scanf("%d", &(newcolumn));
		} while ((newrow > row) || (newcolumn > column));

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) 
		{
			if (Seat_Srv_DeleteByID(p->data.id)) 
			{
				printf("座位删除成功\n");
				ret++;
				List_FreeNode(p);	
			}
		} 
		else 
		{
			printf("座位不存在\n");
		}

		printf("----------------------------\n");
		printf("1.继续删除(d/D)\n2.返回(r/R)\n");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('d' == choice || 'D' == choice);
	
	return ret;

}
