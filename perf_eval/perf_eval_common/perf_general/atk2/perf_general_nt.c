/*
 *  TOPPERS/A-OSBENCH
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Operating System Benchmark
 *
 *  Copyright (C) 2014 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2014 by FUJISOFT INCORPORATED, JAPAN
 *  Copyright (C) 2014 by Sunny Giken Inc., JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id: perf_general_nt.c 653 2016-05-06 19:03:42Z ertl-honda $
 */

#include "perf_general.h"
#include "perf_general_nt.h"
#include "Ioc.h"


/*
 * *************************************
 * ********* 各処理単位の定義 **********
 * *************************************
 */
/* *** ActivateTask *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Act_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP1_Act_Task1_ntnt)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	TerminateTask();
}

TASK(NT_OSAP1_Act_Task1_tnt)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	TerminateTask();
}


TASK(NT_OSAP1_Act_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* Act-1 */
	begin_measure(1);
	ActivateTask(NT_OSAP1_Act_Task1_ntnt);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Act-2 */
	begin_measure(2);
	ActivateTask(NT_OSAP1_Act_Task3_ntnt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}

TASK(NT_OSAP1_Act_Task3_ntnt)
{
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

TASK(NT_OSAP1_Act_Task3_tnt1)
{
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* ResetTask(NT_OSAP1) */
	ActivateTask(NT_OSAP1_Act_ResetTask);

	/* trigger isr2(T_OSAP2_Act_Isr2) */
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_ACT_ISR2_TFN1, NULL_PTR);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	TerminateTask();
}

TASK(NT_OSAP1_Act_Task3_tnt2)
{
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* ResetTask(NT_OSAP2) */
	ActivateTask(NT_OSAP2_Act_ResetTask);

	/* trigger isr2(T_OSAP2_Act_Isr2) */
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_ACT_ISR2_TFN1, NULL_PTR);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	TerminateTask();
}

TASK(NT_OSAP1_Act_Task4)
{
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_Act_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP2_Act_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	TerminateTask();
}

TASK(NT_OSAP2_Act_Task3)
{
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_Act_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* Act-1 */
	begin_measure(1);
	ActivateTask(NT_OSAP2_Act_Task1);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Act-2 */
	begin_measure(2);
	ActivateTask(NT_OSAP2_Act_Task3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}


/* *** TerminateTask *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Ter_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP1_Ter_Task1)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

TASK(NT_OSAP1_Ter_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP1_Ter_Task1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Ter-1 */
	begin_measure(1);
	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_Ter_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP2_Ter_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP3_Ter_Task1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Ter-1 */
	begin_measure(1);
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_Ter_Task1)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}


/* *** ChainTask *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Chain_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* Chain-1 */
	begin_measure(1);
	ChainTask(NT_OSAP1_Chain_Task2);
}

TASK(NT_OSAP1_Chain_Task2)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	TerminateTask();
}

TASK(NT_OSAP1_Chain_ResetTask)
{
	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_Chain_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* Chain-1 */
	begin_measure(1);
	ChainTask(NT_OSAP3_Chain_Task2);
}

TASK(NT_OSAP2_Chain_ResetTask)
{
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_Chain_Task2)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	TerminateTask();
}


/* *** Resource *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Resource_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* GetR-1 */
	begin_measure(1);
	GetResource(NT_TskLevelRes);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Rel-11 */
	begin_measure(2);
	ReleaseResource(NT_TskLevelRes);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	GetResource(NT_TskLevelRes);
	ActivateTask(NT_OSAP1_Resource_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* Rel-12 */
	begin_measure(3);
	ReleaseResource(NT_TskLevelRes);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);

	/* GetR-2 */
	begin_measure(4);
	GetResource(NT_IsrLevelRes);
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);

	/* Rel-21 */
	begin_measure(5);
	ReleaseResource(NT_IsrLevelRes);
	end_measure(5);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);

	GetResource(NT_IsrLevelRes);
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_RES_ISR2_TFN1, NULL_PTR);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);

	/* Rel-22 */
	begin_measure(6);
	ReleaseResource(NT_IsrLevelRes);
	while(check_isr_act == FALSE);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 12);
	TerminateTask();
}

TASK(NT_OSAP1_Resource_Task2)
{
	end_measure(3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	TerminateTask();
}


/* *** Event *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Event_Task1)
{
	EventMaskType event_a_getmask;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP1_Event_Task2);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* GetE-1 */
	begin_measure(2);
	GetEvent(NT_OSAP1_Event_Task2, &event_a_getmask);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	SetEvent(NT_OSAP1_Event_Task2, NT_Event_T2Evt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	TerminateTask();
}

TASK(NT_OSAP1_Event_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* WaitE-2 */
	begin_measure(1);
	WaitEvent(NT_Event_T2Evt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	/* WaitE-1 */
	begin_measure(3);
	WaitEvent(NT_Event_T2Evt);
	end_measure(3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);

	/* ClearE-1 */
	begin_measure(4);
	ClearEvent(NT_Event_T2Evt);
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	TerminateTask();
}


/* *** SetEvent *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_SetE_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP1_SetE_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* SetE-2 */
	begin_measure(1);
	SetEvent(NT_OSAP1_SetE_Task2, NT_Event_T2SetEvt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	ActivateTask(NT_OSAP1_SetE_Task3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 13);
	TerminateTask();
}

TASK(NT_OSAP1_SetE_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	WaitEvent(NT_Event_T2SetEvt);

	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	ClearEvent(NT_Event_T2SetEvt);
	WaitEvent(NT_Event_T2SetEvt);

	ClearEvent(NT_Event_T2SetEvt);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);

	CallTrustedFunction(TRIGGER_SETEVENT_ISR2_TFN1, NULL_PTR);
	while(check_isr_act == FALSE);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 12);
	TerminateTask();
}

TASK(NT_OSAP1_SetE_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);

	/* SetE-1 */
	begin_measure(2);
	SetEvent(NT_OSAP1_SetE_Task2, NT_Event_T2SetEvt);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);

	WaitEvent(NT_Event_T3Evt);
	end_measure(3);

	ClearEvent(NT_Event_T3Evt);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
	TerminateTask();
}


/* *** ISR2_A *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Int_A_Task1)
{
	int_measure_end = FALSE;
	cnt_int = 0;
	int_category = INT_1_2;

	CallTrustedFunction(INIT_TIMER_INT_A_TFN1, NULL_PTR);

	while(1) {
		if (int_measure_end) {
			break;
		}
	}

	CallTrustedFunction(TERMINATE_TIMER_INT_A_TFN1, NULL_PTR);

	TerminateTask();
}


/* *** ISR2_B *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Int_B_Task1)
{
	int_category = INT_3_2;
	int_measure_end = FALSE;
	cnt_int = 0;

	CallTrustedFunction(INIT_TIMER_INT_B_TFN1, NULL_PTR);

	while(1) {
		ActivateTask(NT_OSAP2_Int_B_Task2);
		if (int_measure_end) {
			break;
		}
	}
	CallTrustedFunction(TERMINATE_TIMER_INT_B_TFN1, NULL_PTR);

	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_Int_B_Task2)
{
	TerminateTask();
}


/* *** IOC(NT_OSAP1 -> NT_OSAP2) *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_IOC_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* IocS-1 */
	begin_measure(1);
	IocSend_IOC_QUEUE_NTOSAP1(1);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* IocS-2 */
	begin_measure(2);
	IocWrite_IOC_DEQUEUE_NTOSAP1(2);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	ActivateTask(NT_OSAP2_IOC_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_IOC_Task2)
{
	uint8	ioc_r1_data = 0;
	uint8	ioc_r2_data = 0;
	uint8	ioc_r3_data = 0;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* IocR-1 */
	begin_measure(3);
	IocReceive_IOC_QUEUE_NTOSAP1(&ioc_r1_data);
	end_measure(3);

	/* キューの初期化 */
	IocEmptyQueue_IOC_QUEUE_NTOSAP1();

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	/* IocR-2 */
	begin_measure(4);
	IocReceive_IOC_QUEUE_NTOSAP1(&ioc_r2_data);
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);

	/* IocR-3 */
	begin_measure(5);
	IocRead_IOC_DEQUEUE_NTOSAP1(&ioc_r3_data);
	end_measure(5);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	TerminateTask();
}


/* MC KERNEL */
#ifdef USE_MC_KERNEL
/* *** ActivateTask[MC] *** */
/* NT_OSAP1_M */
TASK(NT_OSAP1_M_Act_Task1)
{
	TaskStateType	task2_state;

	/* CheckPoint(2) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	second_act = FALSE;

	/* ResetTask(NT_OSAP2) */
	ActivateTask(NT_OSAP2_S_Act_ResetTask);

	/* Act-5 */
	begin_measure(1);
	ActivateTask(NT_OSAP2_S_Act_Task2);

	/* CheckPoint(3) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	test_barrier_sync(1, 2);

	/* Act-7 */
	begin_measure(2);
	ActivateTask(NT_OSAP2_S_Act_Task3);
	end_measure(2);

	/* CheckPoint(4) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	test_barrier_sync(2, 2);

	second_act = TRUE;

	begin_measure(3);
	ActivateTask(NT_OSAP2_S_Act_Task3);

	/* CheckPoint(5) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	do {
		GetTaskState(NT_OSAP2_S_Act_Task2, &task2_state);
	} while (task2_state != SUSPENDED);

	/* CheckPoint(6) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	TerminateTask();
}

/* NT_OSAP2_S */
TASK(NT_OSAP2_S_Act_ResetTask)
{
	TerminateTask();
}
	
TASK(NT_OSAP2_S_Act_Task2)
{
	end_measure(1);

	/* CheckPoint(1) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);

	/* synchronization for (Act-7) */
	test_barrier_sync(1, 2);

	/* synchronization for (Act-6) */
	test_barrier_sync(2, 2);

	while(task_act == FALSE);
	task_act = FALSE;

	/* CheckPoint(4) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 4);
	TerminateTask();
}

TASK(NT_OSAP2_S_Act_Task3)
{
	if (second_act == FALSE) {
		/* CheckPoint(2) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
	}
	else {
		end_measure(3);

		/* CheckPoint(3) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 3);
		task_act =TRUE;
	}

	TerminateTask();
}


/* *** SetEvent[MC] *** */
/* NT_OSAP1_M */
TASK(NT_OSAP1_M_SetE_Task1)
{
	/* CheckPoint(2) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP2_S_SetE_Task3);

	/* barrier_sync(1) */
	test_barrier_sync(1, 2);

	/* CheckPoint(3) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	CallTrustedFunction(SETEVENT_MC_TFN1, NULL_PTR);

	/* SetE-4 */
	begin_measure(1);
	SetEvent(NT_OSAP2_S_SetE_Task3, NT_Event_MC_T3Evt);

	/* barrier_sync(2) */
	test_barrier_sync(2, 2);

	/* evtask2_state != RUNNING && evtask3_state != WAITING */
	CallTrustedFunction(SETEVENT_MC_TFN2, NULL_PTR);

	/* CheckPoint(4) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* SetE-5 */
	begin_measure(2);
	SetEvent(NT_OSAP2_S_SetE_Task3, NT_Event_MC_T3Evt);

	/* barrier_sync(3) */
	test_barrier_sync(3, 2);

	/* evtask2_state != RUNNING && evtask3_state != WAITING */
	CallTrustedFunction(SETEVENT_MC_TFN2, NULL_PTR);

	/* CheckPoint(5) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* SetE-6 */
	begin_measure(3);
	SetEvent(NT_OSAP2_S_SetE_Task3, NT_Event_MC_T3Evt);
	end_measure(3);

	/* CheckPoint(6) - OS_CORE_ID_MASTER */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	/* barrier_sync(4) */
	test_barrier_sync(4, 2);

	/* evtask2_state != SUSPENDED */
	CallTrustedFunction(SETEVENT_MC_TFN3, NULL_PTR);

	TerminateTask();
}

/* NT_OSAP2_S */
TASK(NT_OSAP2_S_SetE_Task2)
{
	/* CheckPoint(4) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 4);

	while(evt_act == FALSE);
	evt_act = FALSE;

	/* CheckPoint(7) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 7);

	/* barrier_sync(4) */
	test_barrier_sync(4, 2);

	TerminateTask();
}

TASK(NT_OSAP2_S_SetE_Task3)
{
	/* CheckPoint(1) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);

	/* barrier_sync(1) */
	test_barrier_sync(1, 2);

	WaitEvent(NT_Event_MC_T3Evt);
	end_measure(1);

	/* barrier_sync(2) */
	test_barrier_sync(2, 2);

	ClearEvent(NT_Event_MC_T3Evt);

	/* CheckPoint(2) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
	
	ActivateTask(NT_OSAP2_S_SetE_Task2);

	/* CheckPoint(3) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 3);

	WaitEvent(NT_Event_MC_T3Evt);
	end_measure(2);

	/* barrier_sync(3) */
	test_barrier_sync(3, 2);

	ClearEvent(NT_Event_MC_T3Evt);

	/* CheckPoint(5) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 5);

	WaitEvent(NT_Event_MC_T3Evt);
	ClearEvent(NT_Event_MC_T3Evt);
	evt_act = TRUE;

	/* CheckPoint(6) - OS_CORE_ID_SLAVE */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 6);
	TerminateTask();
}


/* *** ISR2(MC) *** */
/* T_OSAP1_M */
TASK(NT_OSAP1_M_Int_MC_Task1)
{
	int_category = INT_4_2;
	int_measure_end = FALSE;
	cnt_int = 0;

	ActivateTask(NT_OSAP2_S_Int_MC_Task2);

	/* barrier_sync(1) */
	test_barrier_sync(1, 2);

	/* timer_initialize(INT_C_TIMER); */
	CallTrustedFunction(ISR2_MC_TFN1, NULL_PTR);

	GetSpinlock(Int_MC_Spinlock2);

	/* barrier_sync(2) */
	test_barrier_sync(2, 2);

	ReleaseSpinlock(Int_MC_Spinlock2);

	/* int_timer_terminate(INT_C_TIMER); */
	CallTrustedFunction(ISR2_MC_TFN2, NULL_PTR);

	/* isr_task2_state != SUSPENDED */
	CallTrustedFunction(ISR2_MC_TFN3, NULL_PTR);

	TerminateTask();
}

/* NT_OSAP2_S */
TASK(NT_OSAP2_S_Int_MC_Task2)
{
	GetSpinlock(Int_MC_Spinlock2);

	/* barrier_sync(1) */
	test_barrier_sync(1, 2);

	while(1) {
		if (int_measure_end != FALSE)
		break;
	}
	ReleaseSpinlock(Int_MC_Spinlock2);

	/* barrier_sync(2) */
	test_barrier_sync(2, 2);

	TerminateTask();
}


/* *** Spinlock *** */
/* NT_OSAP1_M */
TASK(NT_OSAP1_M_Spin_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	begin_measure(1);
	GetSpinlock(Spin_MC_Spinlock2);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	begin_measure(2);
	ReleaseSpinlock(Spin_MC_Spinlock2);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}


/* *** IOC[MC] *** */
/* NT_OSAP1_M */
TASK(NT_OSAP1_M_IOC_Task1)
{
	TaskStateType	IOC_Task2_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* IocS-1 */
	begin_measure(1);
	IocSend_IOC_QUEUE_NTOSAP1_M(1);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* IocS-2 */
	begin_measure(2);
	IocWrite_IOC_DEQUEUE_NTOSAP1_M(2);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	ActivateTask(NT_OSAP2_S_IOC_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	do {
		GetTaskState(NT_OSAP2_S_IOC_Task2, &IOC_Task2_state);
	} while (IOC_Task2_state != SUSPENDED);

	TerminateTask();
}

/* NT_OSAP2_S */
TASK(NT_OSAP2_S_IOC_Task2)
{
	uint8	ioc_r1_data = 0;
	uint8	ioc_r2_data = 0;
	uint8	ioc_r3_data = 0;

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);

	/* IocR-1 */
	begin_measure(3);
	IocReceive_IOC_QUEUE_NTOSAP1_M(&ioc_r1_data);
	end_measure(3);

	/* キューの初期化 */
	IocEmptyQueue_IOC_QUEUE_NTOSAP1_M();

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);

	/* IocR-2 */
	begin_measure(4);
	IocReceive_IOC_QUEUE_NTOSAP1_M(&ioc_r2_data);
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 3);

	/* IocR-3 */
	begin_measure(5);
	IocRead_IOC_DEQUEUE_NTOSAP1_M(&ioc_r3_data);
	end_measure(5);

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 4);

	TerminateTask();
}
#endif /* USE_MC_KERNEL */

