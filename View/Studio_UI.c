/*
* Copyright(C), 2007-2008, XUPT Univ.	
* 用例编号：TTMS_UC_01	 
* File name: Studio_UI.c			  
* Description : 演出厅用例界面层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/

#include "Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"


static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

/*
标识符：TTMS_SCU_Studio_UI_MgtEnt 
函数功能：界面层管理演出厅的入口函数，显示当前的演出厅数据，并提供演出厅数据添加、修改及删除功能操作的入口。
参数说明：无。
返 回 值：无。
*/
void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		printf(
				"\n==================================================================\n");
		printf(
				"***************************** 放映室列表 ***************************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "名字", "总行数",
				"总列数", "总座位数");
		printf(
				"---------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i)
		{
			printf("%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
		}

		printf("------- 共 %2d 项 ---------------------------- 第 %2d/%2d 页 ---------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]上一页\n");
		printf("[N]下一页\n");
		printf("[A]添加演播厅\n");
		printf("[D]删除演播厅\n");
		printf("[U]更新演播厅\n");
		printf("[S]座位管理\n");
		printf("[R]返回\n");
		printf("\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("请输入ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("请输入ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("请输入ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t)
			;
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

/*
标识符：TTMS_SCU_Studio_UI_Add 
函数功能：用于向系统中添加一个新演出厅数据。
参数说明：无。
返 回 值：整型，成功添加新演出厅的个数。
*/
int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n========================================================\n");
		printf("**********************  新曾演播厅  *********************\n");
		printf("---------------------------------------------------------\n");
		printf("演播厅名:");
		fflush(stdin);
		gets(rec.name);
		printf("座位总行数:");
		scanf("%d", &(rec.rowsCount));
		printf("座位总列数:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = 0;
		printf("=======================================================\n");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("添加成功!\n");
		} else
			printf("添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
标识符：TTMS_SCU_Studio_UI_Mod
函数功能：用于修改系统中现存的一个演出厅数据。
参数说明：id为整型，是需要修改的演出厅ID。
返 回 值：整型，表示是否成功修改了演出厅的标志。
*/
int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("房间不存在!\n按[Enter]键返回!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  更行演播厅  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("演播厅ID:%d\n", rec.id);
	printf("演播厅名[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) 
	{
		do {			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("行数应该 >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("列数应该 >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		} while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
		rec.rowsCount = newrow;
		rec.colsCount = newcolumn;
		rec.seatsCount = seatcount;
	} else {
		printf("座位总行数:");
		scanf("%d", &rec.rowsCount);
		printf("座位总列数:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount = 0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"演播厅信息更新成功!\n按[Enter]键返回!\n");
	} else
		printf("演播厅信息更新失败!\n按[Enter]键返回!\n");

	getchar();
	return rtn;
}

/*
标识符：TTMS_SCU_Studio_UI_Del
函数功能：用于删除系统中现存的一个演出厅数据。
参数说明：id为整型，是需要删除的演出厅ID。
返 回 值：整型，表示是否成功删除了演出厅的标志。
*/
int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("演播厅座位删除成功!\n");
		printf(
				"演播厅删除成功!\n按[Enter]键返回!\n");
		rtn = 1;
	} else {
		printf("演播厅删除失败!\n按[Enter]键返回!\n");
	}

	getchar();
	return rtn;
}
