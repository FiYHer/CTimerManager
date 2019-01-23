#include "TimerManager.h"

using namespace CTimerManagerSpace;

CTimerManager::CTimerManager()
{

}


CTimerManager::~CTimerManager()
{
	//清空所有的定时器
	ClearTimer();
}

//定时器线程
unsigned int __stdcall CTimerManagerSpace::CTimerManager::TimerRunThread(PVOID pThis)
{
	CTimerManager* pTimerManager = (CTimerManager*)pThis;
	if (pThis == NULL)return 0;

	while (true)
	{
		//删除需要删除的定时器
		pTimerManager->InspectDeleteTimer();

		//查看哪一个定时器需要调用了
		if (!pTimerManager->CallBackTimerRun())
			_endthreadex(0);
		
		//缓一缓
		Sleep(20);
	}
}

//添加一个定时器
//#TimerID 定时器的ID
//#TimerMs 定时器的间隔
void CTimerManagerSpace::CTimerManager::AddTimer(int TimerID, unsigned int TimerMs)
{
	//定时器间隔不能为0
	if (TimerMs <= NULL)return;

	//获取当前的时间
	int NowTime = ::GetTickCount();

	//定时器信息填写
	TimerManagerInfo TimerInfoTemp ;
	TimerInfoTemp.timerDelete = false;
	TimerInfoTemp.timerID = TimerID;
	TimerInfoTemp.timerCreateTime = NowTime;
	TimerInfoTemp.timerLastRun = NowTime;
	TimerInfoTemp.timerInterval = TimerMs;

	//加入定时器列表
	m_TimerList.push_back(TimerInfoTemp);

	//第一个定时器的话就开启一个定时器线程，传递this指针
	if (m_TimerList.size() == 1)
	{
		_beginthreadex(NULL, NULL, TimerRunThread, this, NULL, NULL);
	}

}

//删除一个定时器
//#TimerID 定时器ID
void CTimerManagerSpace::CTimerManager::DeleteTimer(int TimerID)
{
	for (TimerManagerInfoList::iterator it = m_TimerList.begin();
		it != m_TimerList.end();it++)
	{
		if (it->timerID == TimerID)
		{
			//仅仅设置删除标志位
			it->timerDelete = true;
			break;
		}
	}
}

//清空所有定时器
void CTimerManagerSpace::CTimerManager::ClearTimer()
{
	for (TimerManagerInfoList::iterator it = m_TimerList.begin();
		it != m_TimerList.end(); it++)
	{
		//全部的定时器的删除标志位都设置为1
		it->timerDelete = true;
	}
}

//检查删除定时器
void CTimerManagerSpace::CTimerManager::InspectDeleteTimer()
{
	for (TimerManagerInfoList::iterator it = m_TimerList.begin(); it != m_TimerList.end();)
	{
		if (it->timerDelete)
		{
			//erase后迭代器会乱序，所以要这样做...
			m_TimerList.erase(it);
			it = m_TimerList.begin();
			continue;
		}
		++it;
	}
}

//需要运行的定时器ID
bool CTimerManagerSpace::CTimerManager::CallBackTimerRun()
{
	if (m_TimerList.size() == NULL)return false;

	int NowTimer = ::GetTickCount();

	for (TimerManagerInfoList::iterator it = m_TimerList.begin();
		it != m_TimerList.end(); it++)
	{
		if (NowTimer - it->timerLastRun >= it->timerInterval)
		{
			//现在的时间减去上一次运行的时间得到一个时间间隔
			//如果这个时间间隔大于定时器运行间隔
			//那样子的话就运行这个定时器
			it->timerLastRun = NowTimer;
			if (OnTimer(it->timerID) == NULL)
			{
				//返回空就删除这个定时器
				it->timerDelete = true;
			}
			continue;
		}
	}
	return true;
}

