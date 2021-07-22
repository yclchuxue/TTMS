#ifndef SCHEDPERSIST_H_
#define SCHEDPERSIST_H_

	#include "../Service/Schedule.h"


int Schedule_Perst_SelectByID(int ID, schedule_t *buf);

//将所有演出计划信息建立成一条链表，输入list为演出计划信息链表的头指针
//输出 提示建立链表时，申请空间失败，返回查找到的记录数目
int Schedule_Perst_SelectAll(schedule_list_t list); 

//根据ID载入演出计划
//从演出计划数据文件中载入与指定剧目相关联的演出计划信息构成链表list
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);

//储存新演出计划
//向演出计划文件Schedule.dat的末尾添加一条新演出计划
int Schedule_Perst_Insert(schedule_t * date);
//更新演出计划
//更新演出计划数据文件Schedule.dat中的一条演出计划信息
int Schedule_Perst_Update(const schedule_t * date);

//根据ID去除演出计划
//去除演出计划数据文件Schedule.dat中指定ID的演出计划
int Schedule_Perst_RemByID(int id);

int Schedule_Perst_SelectByID_ticket(int id,schedule_t *buf);






#endif // SCHEDPERSIST_H_
