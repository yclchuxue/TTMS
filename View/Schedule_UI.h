#ifndef SCHEDULE_UI_H_
	#define SCHEDULE_UI_H_

	#include "../Common/List.h"
	#include "../Common/Common.h"
	#include "../Service/Schedule.h"
	#include "../View/Play_UI.h"

	static const int SCHEDULE_PAGE_SIZE=5;

	
	
	void Schedule_UI_MgtEntry(int play_id);
	int Schedule_UI_Add(int play_id);
	int Schedule_UI_Modify(int id);
	int Schedule_UI_Delete(int id);
	
	void Schedule_UI_ListAll(void);
	
	//按照ID号查询演出计划信息,输入待查找的剧目ID号，输出查找到的剧目信息。
	//返回 0表示未找到，1表示找到了
	int Schedule_UI_Query(int id);

#endif // 
