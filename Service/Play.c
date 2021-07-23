#include "../View/Play_UI.h"
#include "Play.h"

/*
标识符：TTMS_SCU_Play_Srv_Add
函数功能：调用Play_Perst_Insert添加剧目
参数说明：data为play_t类型指针，表示待存储的剧目信息
返回值：整型，返回1表示添加成功，否则表示失败
*/ 
int Play_Srv_Add(play_t *data)
{
	int i = Play_Perst_Insert(data);
	if(i == 1)
		return 1;
	else
		return -1;
}

/*
标识符：TTMS_SCU_Play_Srv_FetchAll
函数功能：调用Play_Perst_SelectAll获取全部剧目信息
参数说明：list为play_list_t类型指针，表示记录所有剧目信息的链表头指针
返回值：返回值>0表示载入剧目数量， 为0表示失败
*/ 
int Play_Srv_FetchAll(play_list_t list)
{
	return Play_Perst_SelectAll(list);	
}

/*
标识符：TTMS_SCU_Play_Srv_FetchByID
函数功能：调用 Play_Perst_SelectByID获取剧目信息
参数说明：id为带获取剧目id，buf用来存放剧目信息
返回值：返回1表示成功，否则失败
*/ 
int Play_Srv_FetchByID(int id, play_t *buf)
{
	return Play_Perst_SelectByID(id, buf);
}

/*
标识符：TTMS_SCU_Play_Srv_Mod
函数功能：调用 Play_Perst_Modify修改剧目信息
参数说明：data存储剧目信息
返回值：返回1表示成功，否则失败 
*/
int Play_Srv_Modify(const play_t *data)
{
	return Play_Perst_Update(data);
}

/*
标识符：TTMS_SCU_Play_Srv_DelByID
函数功能：调用Play_Perst_RemByID删除剧目
参数说明：id为待删除剧目id
返回值：1表示成功，否则失败
*/ 
int Play_Srv_DelectByID(int id)
{
	return Play_Perst_RemByID(id);
}

int Play_Srv_FetchByName(play_list_t list,char condt[])
{
	return Play_Perst_SelectByName(list,condt);
}
