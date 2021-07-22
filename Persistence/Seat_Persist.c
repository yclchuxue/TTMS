#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";
static const char SEAT_KEY_NAME[] = "Seat";

/*
函数功能：用于向文件中添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/ 
int Seat_Perst_Insert(seat_t *data) {   
	assert(NULL!=data);
	
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	
	int rtn = 0;

	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(seat_t), 1, fp);

	fclose(fp);
	return rtn;
}

/*
标识符：TTMS_SCU_Seat_Perst_InsertBatch
函数功能：用于向文件中添加一批座位数据。
参数说明：list为seat_list_t类型，表示需要添加的一批座位的链表头指针。
返 回 值：整型，表示成功添加一批座位的个数。
*/
int Seat_Perst_InsertBatch(seat_list_t list) {
	seat_node_t *p;
	assert(NULL!=list);
	
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;

	if (fp == NULL)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	List_ForEach(list,p)
	{
		fwrite(&(p->data), sizeof(seat_t), 1, fp);
		rtn++;
	}
	fclose(fp);
	return rtn;
}

/*
标识符：TTMS_SCU_Seat_Perst_Update
函数功能：用于在文件中更新一个座位数据。
参数说明：data为seat_t类型指针，表示需要更新的座位数据结点。
返 回 值：整型，表示是否成功更新了座位的标志。
*/
int Seat_Perst_Update(const seat_t *seatdata) {
	
	seat_t buf;
	int found = 0;
	
	assert(NULL!=seatdata);
	
	FILE *fp = fopen(SEAT_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("文件 %s无法打开!\n", SEAT_DATA_FILE);
		return 0;
	}
	
	while (!feof(fp)) {
		if (fread(&buf, sizeof(seat_t), 1, fp))
		{
			if (buf.id == seatdata->id)
			{
				fseek(fp, -sizeof(seat_t), SEEK_CUR);
				fwrite(seatdata, sizeof(seat_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	
	fclose(fp);
	
	return found;

}

/*
识符：TTMS_SCU_Seat_Perst_DelByID
函数功能：用于从文件中删除一个座位的数据。
参数说明：参数ID为整型，表示需要删除的座位ID。 
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteByID(int ID) {
	
	int ret = 0;
	seat_t buf;

	FILE *fp1;
	FILE *fp2;
	rename("Seat.dat", "SeatTmp.dat");

	fp1 = fopen("SeatTmp.dat", "rb");
	fp2 = fopen("Seat.dat", "wb");

	if(fp1 == NULL)
    {
        printf("文件Seat.dat打开失败\n");
        return -1;
    }

	while(!feof(fp1))
	{
		if(fread(&buf, sizeof(seat_t), 1, fp1))
		{
			if(buf.id != ID)
				fwrite(&buf, sizeof(seat_t), 1, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);

	remove("SeatTmp.dat");

	return 1;
}

/*
标识符：TTMS_SCU_Seat_Perst_DelAllByID
函数功能：根据编号用于从文件中删除座位数据。
参数说明：参数roomID为整型，表示演出厅ID。 
返 回 值：整型，表示是否成功删除了座位的标志。
*/ 
int Seat_Perst_DeleteAllByRoomID(int roomID) {
	
	seat_t buf;
	int found = 0;

	FILE *fp1, *fp2;
	fp1 = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fp2 = fopen(SEAT_DATA_FILE, "wb");

	rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE);

	fp1 = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fp2 = fopen(SEAT_DATA_FILE, "wb");

	if (fp1 == NULL)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}
	if (fp2 == NULL)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}


	while (!feof(fp1))
	{
		if (fread(&buf, sizeof(seat_t), 1, fp1))
		{
			if (roomID == buf.roomID)
			{
				found += 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);
	
	remove(SEAT_DATA_TEMP_FILE);
	return found;
}

/*
标识符：TTMS_SCU_Studio_Perst_SelByID
函数功能：用于从文件中载入一个座位的数据。
参数说明：第一个参数ID为整型，表示需要载入数据的座位ID；第二个参数buf为seat_t指针，指向载入座位数据的指针。
返 回 值：整型，表示是否成功载入了座位的标志。
*/
int Seat_Perst_SelectByID(int ID, seat_t *buf) {
	
	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	seat_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp))
		{
			if (ID == data.id)
			{
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelAll
函数功能：用于从文件中载入所有座位数据。
参数说明：list为seat_list_t类型，表示将要载入的座位链表头指针。
返 回 值：整型，成功载入座位的个数。
*/
int Seat_Perst_SelectAll(seat_list_t list) {
	
	int ret = 0;
	seat_node_t *newNode;
	seat_t data;
	char *new = "Seat.dat";

	FILE *fp = fopen(new, "rb");
	if(!fp)
	{
		printf("文件Seat.dat 打开失败!\n");
		return ret;
	}

	List_Free(list, seat_node_t);               


	while (!feof(fp))
	{
		if (fread(&data, sizeof(seat_t), 1, fp))
		{
			newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
			newNode->data = data;
			List_AddTail(list, newNode);               
			ret++;
		}
	}

	fclose(fp);
	return ret;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelByRoomID
函数功能：用于在文件中根据演出厅ID载入所有座位数据。
参数说明：第一个参数list为seat_list_t类型，表示将要载入的座位链表头指针，第二个参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示成功载入了演出厅座位的个数。
*/
int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
	int ret = 0;
	seat_node_t *newNode;
	seat_t data;

	List_Free(list, seat_node_t);

	FILE *fp = fopen("Seat.dat", "rb");
	if(!fp)
	{
		printf("文件Seat.dat 无法打开!\n");
		return ret;
	}

	while(!feof(fp))
	{
		if(fread(&data, sizeof(seat_t), 1, fp))
		{
			if(data.roomID == roomID)              
			{
				newNode = (seat_node_t *)malloc(sizeof(seat_node_t));
				newNode->data = data;
				List_AddTail(list, newNode);
				ret++;
			}
		}
	}

	fclose(fp);

	return ret;
}
