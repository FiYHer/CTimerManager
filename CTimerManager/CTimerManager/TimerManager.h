/*
定时器实现
*/
#pragma once
#include <list>

#include <windows.h>
#include <process.h>
using namespace std;


///定时器的命名空间
namespace CTimerManagerSpace
{

	//定时器信息结构体
	typedef struct strTimerManagerInfo
	{
		unsigned int timerInterval;//定时器运行间隔
		unsigned int timerCreateTime;//定时器创建时间
		unsigned int timerLastRun;//定时器上一次运行的时间
		int timerID;//定时器的ID号
		bool timerDelete;//定时器是不是要删除
		strTimerManagerInfo()
		{
			timerInterval = 0;
			timerCreateTime = 0;
			timerLastRun = 0;
			timerID = -1;
			timerDelete = false;
		}
	}TimerManagerInfo,*PTimerManagerInfo;
	
	typedef list<TimerManagerInfo> TimerManagerInfoList;

	//定时器类
	class CTimerManager
	{
	private:
		//定时器列表
		TimerManagerInfoList m_TimerList;
	public:
		CTimerManager();
		virtual ~CTimerManager();

		CTimerManager(const CTimerManager&) = delete;
		CTimerManager& operator=(const CTimerManager&) = delete;
		CTimerManager(CTimerManager&&) = delete;
	public:
		//纯虚函数
		virtual int OnTimer(int)=0;

		//定时器线程
		static unsigned int __stdcall TimerRunThread(PVOID);
	public:
		//添加一个定时器
		void AddTimer(int,unsigned int);

		//删除一个定时器
		void DeleteTimer(int);

		//清空所有定时器
		void ClearTimer();

		//检查删除定时器
		void InspectDeleteTimer();

		//需要运行的定时器ID
		bool CallBackTimerRun();

	};

}

