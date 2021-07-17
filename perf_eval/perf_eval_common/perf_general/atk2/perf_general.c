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
 *  $Id: perf_general.c 646 2016-05-06 05:07:47Z ertl-honda $
 */


#include "perf_general.h"
#if defined(USE_MC_KERNEL) || defined(USE_SCALABILITYCLASS3)
#include "Ioc.h"
#endif


/* 割込み判定用フラグ */
volatile boolean check_isr_act = FALSE;

/* タスク判別フラグ(MC,SC3用) */
#ifdef USE_MC_KERNEL
volatile boolean task_act   = FALSE;
volatile boolean second_act = FALSE; 
volatile boolean evt_act    = FALSE;
#endif /* USE_MC_KERNEL */


/* Prototype Declaration */
LOCAL_INLINE void record_hist(uint32 ustime, uint16 *histarea);


/*
 * *************************************
 * ********* 各処理単位の定義 **********
 * *************************************
 */
/* *** ActivateTask *** */
TASK(Act_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	TerminateTask();
}

TASK(Act_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* Act-1 */
	begin_measure(1);
	ActivateTask(Act_Task1);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Act-2 */
	begin_measure(2);
	ActivateTask(Act_Task3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	TerminateTask();
}

TASK(Act_Task3)
{
	end_measure(2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	check_isr_act = FALSE;
	test_int_raise(INTNO_ACTISR2_1);
	while(check_isr_act == FALSE) {
	}

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	TerminateTask();
}

TASK(Act_Task4)
{
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	TerminateTask();
}


ISR(Act_Isr2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_ACTISR2_1);

	/* Act-3 */
	begin_measure(3);
	ActivateTask(Act_Task4);
	end_measure(3);

	/* Act-4 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	begin_measure(4);
}

#ifdef USE_SCALABILITYCLASS3
/* T_OSAP2 */
TASK(T_OSAP2_Act_Task2_tnt1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* ResetTask(NT_OSAP1) */
	ActivateTask(NT_OSAP1_Act_ResetTask);

	/* Act-1 */
	begin_measure(1);
	ActivateTask(NT_OSAP1_Act_Task1_tnt);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Act-2 */
	begin_measure(2);
	ActivateTask(NT_OSAP1_Act_Task3_tnt1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	TerminateTask();
}

TASK(T_OSAP2_Act_Task2_tnt2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* ResetTask(NT_OSAP2) */
	ActivateTask(NT_OSAP2_Act_ResetTask);

	/* Act-1 */
	begin_measure(1);
	ActivateTask(NT_OSAP1_Act_Task1_tnt);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Act-2 */
	begin_measure(2);
	ActivateTask(NT_OSAP1_Act_Task3_tnt2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	TerminateTask();
}

ISR(T_OSAP2_Act_Isr2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_ACTISR2_2);

	/* Act-3 */
	begin_measure(3);
	ActivateTask(NT_OSAP1_Act_Task4);
	end_measure(3);

	/* Act-4 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	begin_measure(4);
}

StatusType TRUSTED_act_isr2_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_ACTISR2_2);
	return(E_OK);
}
#endif /* USE_SCALABILITYCLASS3 */


/* *** TerminateTask *** */
TASK(Ter_Task1)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

TASK(Ter_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(Ter_Task1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Ter-1 */
	begin_measure(1);

	TerminateTask();
}

#ifdef USE_SCALABILITYCLASS3
/* T_OSAP2 */
TASK(T_OSAP2_Ter_Task2_1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP1_Ter_Task1);

	/* reset_task(NT_OSAP1) */
	ActivateTask(NT_OSAP1_Ter_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	/* Ter-1 */
	begin_measure(1);

	TerminateTask();
}

TASK(T_OSAP2_Ter_Task2_2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP1_Ter_Task1);

	/* reset_task(NT_OSAP2) */
	ActivateTask(NT_OSAP2_Ter_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	/* Ter-1 */
	begin_measure(1);

	TerminateTask();
}
#endif /* USE_SCALABILITYCLASS3 */


/* *** ChainTask *** */
TASK(Chain_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	/* Chain-1 */
	begin_measure(1);
	ChainTask(Chain_Task2);
}

TASK(Chain_Task2)
{
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

#ifdef USE_SCALABILITYCLASS3
/* T_OSAP2 */
TASK(T_OSAP2_Chain_Task1_1)
{
	/* ResetTask(NT_OSAP1) */
	ActivateTask(NT_OSAP1_Chain_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	/* Chain-1 */
	begin_measure(1);
	ChainTask(NT_OSAP1_Chain_Task2);
}

TASK(T_OSAP2_Chain_Task1_2)
{
	/* ResetTask(NT_OSAP2) */
	ActivateTask(NT_OSAP2_Chain_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	/* Chain-1 */
	begin_measure(1);
	ChainTask(NT_OSAP1_Chain_Task2);
}
#endif /* USE_SCALABILITYCLASS3 */


/* *** Interrupt *** */
TASK(Interrupt_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	/* Dis-1 */
	begin_measure(1);
	DisableAllInterrupts();
	end_measure(1);

	/* Ena-1 */
	begin_measure(2);
	EnableAllInterrupts();
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Sus-1 */
	begin_measure(3);
	SuspendAllInterrupts();
	end_measure(3);

	/* Res-1 */
	begin_measure(4);
	ResumeAllInterrupts();
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}


/* *** Resource *** */
TASK(Resource_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	/* GetR-1 */
	begin_measure(1);
	GetResource(TskLevelRes);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	/* Rel-11 */
	begin_measure(2);
	ReleaseResource(TskLevelRes);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	GetResource(TskLevelRes);
	ActivateTask(Resource_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	/* Rel-12 */
	begin_measure(3);
	ReleaseResource(TskLevelRes);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	/* GetR-2 */
	begin_measure(4);
	GetResource(IsrLevelRes);
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	/* Rel-21 */
	begin_measure(5);
	ReleaseResource(IsrLevelRes);
	end_measure(5);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	GetResource(IsrLevelRes);
	check_isr_act = FALSE;
	test_int_raise(INTNO_RESOURCE);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	/* Rel-22 */
	begin_measure(6);
	ReleaseResource(IsrLevelRes);
	while(check_isr_act == FALSE) {
	}

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 12);
	TerminateTask();
}

TASK(Resource_Task2)
{
	end_measure(3);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	TerminateTask();
}

ISR(Resource_Isr2)
{
	end_measure(6);
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_RESOURCE);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
}

#ifdef USE_SCALABILITYCLASS3
StatusType TRUSTED_res_isr2_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_RESOURCE);
	return(E_OK);
}
#endif /* USE_SCALABILITYCLASS3 */


/* *** Event *** */
TASK(Event_Task1)
{
	EventMaskType event_a_getmask;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(Event_Task2);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	/* GetE-1 */
	begin_measure(2);
	GetEvent(Event_Task2, &event_a_getmask);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	SetEvent(Event_Task2, Event_T2Evt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	TerminateTask();
}

TASK(Event_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	/* WaitE-2 */
	begin_measure(1);
	WaitEvent(Event_T2Evt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	/* WaitE-1 */
	begin_measure(3);
	WaitEvent(Event_T2Evt);
	end_measure(3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	/* ClearE-1 */
	begin_measure(4);
	ClearEvent(Event_T2Evt);
	end_measure(4);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	TerminateTask();
}


/* *** SetEvent *** */
TASK(SetE_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(SetE_Task2);

	/* SetE-2 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	begin_measure(1);
	SetEvent(SetE_Task2, Event_T2SetEvt);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	ActivateTask(SetE_Task3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 13);
	TerminateTask();
}

TASK(SetE_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	WaitEvent(Event_T2SetEvt);

	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	ClearEvent(Event_T2SetEvt);
	WaitEvent(Event_T2SetEvt);

	ClearEvent(Event_T2SetEvt);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);
	check_isr_act = FALSE;
	test_int_raise(INTNO_SET_EVENT_1);
	while(check_isr_act == FALSE){
	}

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 12);
	TerminateTask();
}

TASK(SetE_Task3)
{
	/* SetE-1 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	begin_measure(2);
	SetEvent(SetE_Task2, Event_T2SetEvt);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
	WaitEvent(Event_T3Evt);
	end_measure(3);

	ClearEvent(Event_T3Evt);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
	TerminateTask();
}

ISR(SetE_Isr2)
{
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_SET_EVENT_1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	SetEvent(SetE_Task3, Event_T3Evt);
  	/* SetE-3 */
	begin_measure(3);
}

#ifdef USE_SCALABILITYCLASS3
/* T_OSAP2 */
ISR(T_OSAP2_SetE_Isr2)
{
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_SET_EVENT_2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);

	SetEvent(NT_OSAP1_SetE_Task3, NT_Event_T3Evt);
	begin_measure(3);
}

StatusType TRUSTED_setevent_isr2_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_SET_EVENT_2);
	return(E_OK);
}
#endif /* USE_SCALABILITYCLASS3 */


/* *** ISR2_A *** */
TASK(Int_A_Task1)
{
	int_measure_end = FALSE;
	cnt_int = 0;
	int_category = INT_1_1;

	int_timer_initialize(INT_A_TIMER);

	while(1) {
		if (int_measure_end) {
			break;
		}
	}

	int_timer_terminate(INT_A_TIMER);

	TerminateTask();
}

ISR(Int_A_Isr2)
{
	uint32  ustime;
	volatile int j;

	int_get_100ntime(&ustime, INT_A_TIMER);

	record_hist(ustime, histarea[int_category - 1]);

	test_clear_int_req(INTNO_INT_A_TIMER);

	cnt_int++;

	for (j = 0; j < NO_LOOP1; j++) {
	}

	if (cnt_int >= LOOP_COUNT) {
		int_measure_end = TRUE;

		if (int_category == INT_2) {
			ActivateTask(PrintTask);
		}
	}
	invalidate_icache();
}

#ifdef USE_SCALABILITYCLASS3
StatusType TRUSTED_isr2_a_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	int_timer_initialize(INT_A_TIMER);
	return(E_OK);
}

StatusType TRUSTED_isr2_a_tfn2(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	int_timer_terminate(INT_A_TIMER);
	return(E_OK);
}
#endif /* USE_SCALABILITYCLASS3 */


/* *** ISR2_B *** */
ISR(Int_B_Isr2)
{
	uint32  ustime;
	volatile int j;

	int_get_100ntime(&ustime, INT_B_TIMER);
	record_hist(ustime, histarea[int_category - 1]);

	test_clear_int_req(INTNO_INT_B_TIMER);
	cnt_int++;

	for (j = 0; j < NO_LOOP1; j++) {
	}

	if (cnt_int >= LOOP_COUNT) {
		int_measure_end = TRUE;
	}
	invalidate_icache();
}

TASK(Int_B_Task1)
{
	int_category = INT_3_1;
	int_measure_end = FALSE;
	cnt_int = 0;

	int_timer_initialize(INT_B_TIMER);

	while(1) {
		ActivateTask(Int_B_Task2);
		if (int_measure_end) {
			break;
		}
	}
	int_timer_terminate(INT_B_TIMER);

	TerminateTask();
}

TASK(Int_B_Task2)
{
	TerminateTask();
}

#ifdef USE_SCALABILITYCLASS3
StatusType TRUSTED_isr2_b_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	int_timer_initialize(INT_B_TIMER);
	return(E_OK);
}

StatusType TRUSTED_isr2_b_tfn2(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	int_timer_terminate(INT_B_TIMER);
	return(E_OK);
}
#endif /* USE_SCALABILITYCLASS3 */


TASK(PrintTask)
{
	int_timer_terminate(INT_A_TIMER);

#ifndef USE_SCALABILITYCLASS3
	test_outputlog_0("== Int-1 ==");
	print_hist(1);
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Int-1(T_OSAP1) ==");
	print_hist(1);
	test_outputlog_0("== Int-1(NT_OSAP1) ==");
	print_hist(2);
#endif /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Int-2 ==");
	print_hist(3);
#ifndef USE_SCALABILITYCLASS3
	test_outputlog_0("== Int-3 ==");
	print_hist(4);
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Int-3(T_OSAP1) ==");
	print_hist(4);
	test_outputlog_0("== Int-3(NT_OSAP1) ==");
	print_hist(5);
#endif /* USE_SCALABILITYCLASS3 */
#ifdef USE_MC_KERNEL
#ifndef USE_SCALABILITYCLASS3
	test_outputlog_0("== Int-4 ==");
	print_hist(6);
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Int-4(T_OSAP1_M) ==");
	print_hist(6);
	test_outputlog_0("== Int-4(NT_OSAP1_M) ==");
	print_hist(7);
#endif /* USE_SCALABILITYCLASS3 */
#endif /* USE_MC_KERNEL */

	test_outputlog_0("All evaluation items were measured.");

#ifndef USE_MC_KERNEL
	ShutdownOS(E_OK);
#else  /* USE_MC_KERNEL */
	ShutdownAllCores(E_OK);
#endif /* USE_MC_KERNEL */
}


/* *** IOC *** */
#if defined(USE_MC_KERNEL) || defined(USE_SCALABILITYCLASS3)
/* OSAP1 */
TASK(OSAP1_IOC_Task1)
{
	/* IocS-1 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	IocSend_IOC_QUEUE_OSAP1(1);
	end_measure(1);

	/* IocS-2 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(2);
	IocWrite_IOC_DEQUEUE_OSAP1(2);
	end_measure(2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	ActivateTask(OSAP2_IOC_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);

	TerminateTask();
}

/* OSAP2 */
TASK(OSAP2_IOC_Task2)
{
	uint8 ioc_r1_data = 0;
	uint8 ioc_r2_data = 0;
	uint8 ioc_r3_data = 0;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* IocR-1 */
	begin_measure(3);
	IocReceive_IOC_QUEUE_OSAP1(&ioc_r1_data);
	end_measure(3);

	/* キューの初期化 */
	IocEmptyQueue_IOC_QUEUE_OSAP1();

	/* IocR-2 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	begin_measure(4);
	IocReceive_IOC_QUEUE_OSAP1(&ioc_r2_data);
	end_measure(4);

	/* IocR-3 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	begin_measure(5);
	IocRead_IOC_DEQUEUE_OSAP1(&ioc_r3_data);
	end_measure(5);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);

	TerminateTask();
}
#endif /* USE_MC_KERNEL || USE_SCALABILITYCLASS3 */


/* MC KERNEL */
#ifdef USE_MC_KERNEL
/* *** ActivateTask[MC] *** */
/* OSAP1_M */
TASK(OSAP1_M_Act_Task1)
{
	TaskStateType	task2_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	second_act = FALSE;

	/* Act-5 */
	begin_measure(1);
	ActivateTask(OSAP2_S_Act_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Act-7*/
	test_barrier_sync(1, 2);
	begin_measure(2);
	ActivateTask(OSAP2_S_Act_Task3);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	test_barrier_sync(2, 2);
	second_act = TRUE;

	begin_measure(3);
	ActivateTask(OSAP2_S_Act_Task3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	do {
		GetTaskState(OSAP2_S_Act_Task2, &task2_state);
	} while (task2_state != SUSPENDED);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	TerminateTask();
}

/* OSAP2_S */
TASK(OSAP2_S_Act_Task2)
{
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);

	/* synchronization for (Act-7) */
	test_barrier_sync(1, 2);

	/* synchronization for (Act-6) */
	test_barrier_sync(2, 2);

	while(task_act == FALSE);
	task_act = FALSE;
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 4);

	TerminateTask();
}

TASK(OSAP2_S_Act_Task3)
{
	if (second_act == FALSE) {
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
	}
	else {
		end_measure(3);
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 3);
		task_act =TRUE;
	}

	TerminateTask();
}


/* *** SetEvent[MC] *** */
/* OSAP1_M */
TASK(OSAP1_M_SetE_Task1)
{
	TaskStateType	evtask2_state;
	TaskStateType	evtask3_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(OSAP2_S_SetE_Task3);
	test_barrier_sync(1, 2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	while(test_is_core_idle(OS_CORE_ID_SLAVE) == FALSE) {
	}

	/* SetE-4 */
	begin_measure(1);
	SetEvent(OSAP2_S_SetE_Task3, Event_MC_T3Evt);
	test_barrier_sync(2, 2);

	do {
		GetTaskState(OSAP2_S_SetE_Task2, &evtask2_state);
		GetTaskState(OSAP2_S_SetE_Task3, &evtask3_state);
	} while ((evtask2_state != RUNNING) && (evtask3_state != WAITING));
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* SetE-5 */
	begin_measure(2);
	SetEvent(OSAP2_S_SetE_Task3, Event_MC_T3Evt);
	test_barrier_sync(3, 2);

	do {
		GetTaskState(OSAP2_S_SetE_Task2, &evtask2_state);
		GetTaskState(OSAP2_S_SetE_Task3, &evtask3_state);
	} while ((evtask2_state != RUNNING) && (evtask3_state != WAITING));
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* SetE-6 */
	begin_measure(3);
	SetEvent(OSAP2_S_SetE_Task3, Event_MC_T3Evt);
	end_measure(3);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	test_barrier_sync(4, 2);

	do {
		GetTaskState(OSAP2_S_SetE_Task2, &evtask2_state);
	} while(evtask2_state != SUSPENDED);

	TerminateTask();
}

#ifdef USE_SCALABILITYCLASS3
StatusType TRUSTED_setevent_mc_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	while(test_is_core_idle(OS_CORE_ID_SLAVE) == FALSE) {
	}
	return(E_OK);
}

StatusType TRUSTED_setevent_mc_tfn2(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	TaskStateType	evtask2_state;
	TaskStateType	evtask3_state;

	do {
		GetTaskState(NT_OSAP2_S_SetE_Task2, &evtask2_state);
		GetTaskState(NT_OSAP2_S_SetE_Task3, &evtask3_state);
	} while ((evtask2_state != RUNNING) && (evtask3_state != WAITING));

	return(E_OK);
}

StatusType TRUSTED_setevent_mc_tfn3(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	TaskStateType	evtask2_state;

	do {
		GetTaskState(OSAP2_S_SetE_Task2, &evtask2_state);
	} while(evtask2_state != SUSPENDED);

	return(E_OK);
}

#endif /* USE_SCALABILITYCLASS3 */

/* OSAP2_S */
TASK(OSAP2_S_SetE_Task2)
{
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 4);

	while(evt_act == FALSE) {
	}
	evt_act = FALSE;

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 7);
	test_barrier_sync(4, 2);

	TerminateTask();
}

TASK(OSAP2_S_SetE_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);
	test_barrier_sync(1, 2);
	WaitEvent(Event_MC_T3Evt);
	end_measure(1);
	test_barrier_sync(2, 2);
	ClearEvent(Event_MC_T3Evt);

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
	ActivateTask(OSAP2_S_SetE_Task2);

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 3);
	WaitEvent(Event_MC_T3Evt);
	end_measure(2);
	test_barrier_sync(3, 2);
	ClearEvent(Event_MC_T3Evt);

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 5);
	WaitEvent(Event_MC_T3Evt);
	ClearEvent(Event_MC_T3Evt);
	evt_act = TRUE;

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 6);

	TerminateTask();
}


/* *** Spinlock[MC] *** */
/* OSAP1_M */
TASK(OSAP1_M_Spin_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	GetSpinlock(Spin_MC_Spinlock1);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(2);
	ReleaseSpinlock(Spin_MC_Spinlock1);
	end_measure(2);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	TerminateTask();
}


/* *** ISR2[MC] *** */
/* OSAP1_M */
TASK(OSAP1_M_Int_MC_Task1)
{
	TaskStateType	isr_task2_state;

	int_category = INT_4_1;
	int_measure_end = FALSE;
	cnt_int = 0;

	ActivateTask(OSAP2_S_Int_MC_Task2);

	test_barrier_sync(1, 2);

	int_timer_initialize(INT_C_TIMER);

	GetSpinlock(Int_MC_Spinlock1);

	test_barrier_sync(2, 2);
	ReleaseSpinlock(Int_MC_Spinlock1);

	int_timer_terminate(INT_C_TIMER);

	do {
		GetTaskState(OSAP2_S_Int_MC_Task2, &isr_task2_state);
	} while(isr_task2_state != SUSPENDED);

	TerminateTask();
}

ISR(OSAP1_M_Int_MC_Isr2)
{
	uint32  ustime;
	volatile int j;

	int_get_100ntime(&ustime, INT_C_TIMER);
	test_clear_int_req(INTNO_INT_MC_TIMER);
	record_hist(ustime, histarea[int_category - 1]);
	cnt_int++;

	for (j = 0; j < NO_LOOP1; j++) {
	}

	if (cnt_int >= LOOP_COUNT) {
	  int_measure_end = TRUE;
	}
	invalidate_icache();
}

#ifdef USE_SCALABILITYCLASS3
StatusType TRUSTED_isr2_mc_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	int_timer_initialize(INT_C_TIMER);

	return(E_OK);
}

StatusType TRUSTED_isr2_mc_tfn2(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	int_timer_terminate(INT_C_TIMER);

	return(E_OK);
}

StatusType TRUSTED_isr2_mc_tfn3(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	TaskStateType	isr_task2_state;

	do {
		GetTaskState(NT_OSAP2_S_Int_MC_Task2, &isr_task2_state);
	} while(isr_task2_state != SUSPENDED);

	return(E_OK);
}
#endif /* USE_SCALABILITYCLASS3 */

/* T_OSAP2_S */
TASK(OSAP2_S_Int_MC_Task2)
{
	GetSpinlock(Int_MC_Spinlock1);
	test_barrier_sync(1, 2);

	while(1) {
		if (int_measure_end != FALSE)
		break;
	}
	ReleaseSpinlock(Int_MC_Spinlock1);
	test_barrier_sync(2, 2);

	TerminateTask();
}


/* *** IOC[MC] *** */
/* OSAP1_M */
TASK(OSAP1_M_IOC_Task1)
{
	TaskStateType	IOC_Task2_state;

	/* IocS-1 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	IocSend_IOC_QUEUE_OSAP1_M(1);
	end_measure(1);

	/* IocS-2 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(2);
	IocWrite_IOC_DEQUEUE_OSAP1_M(2);
	end_measure(2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	ActivateTask(OSAP2_S_IOC_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	do {
		GetTaskState(OSAP2_S_IOC_Task2, &IOC_Task2_state);
	} while (IOC_Task2_state != SUSPENDED);

	TerminateTask();
}

/* T_OSAP2_S */
TASK(OSAP2_S_IOC_Task2)
{
	uint8 ioc_r1_data = 0;
	uint8 ioc_r2_data = 0;
	uint8 ioc_r3_data = 0;

	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);

	/* IocR-1 */
	begin_measure(3);
	IocReceive_IOC_QUEUE_OSAP1_M(&ioc_r1_data);
	end_measure(3);

	/* キューの初期化 */
	IocEmptyQueue_IOC_QUEUE_OSAP1_M();

	/* IocR-2 */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
	begin_measure(4);
	IocReceive_IOC_QUEUE_OSAP1_M(&ioc_r2_data);
	end_measure(4);

	/* IocR-3 */
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 3);
	begin_measure(5);
	IocRead_IOC_DEQUEUE_OSAP1_M(&ioc_r3_data);
	end_measure(5);
	SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 4);

	TerminateTask();
}


#ifdef SLAVE_IDLE_TASK
/* IdleTask(SlaveCore) */
TASK(S_IdleTask)
{
	idle_task_process(OS_CORE_ID_SLAVE);
	TerminateTask();
}
#endif /* SLAVE_IDLE_TASK */
#endif /* USE_MC_KERNEL */


/*
 *  時間の頻度をヒストグラムとして記録する関数
 */
LOCAL_INLINE void
record_hist(uint32 ustime, uint16 *histarea)
{
	uint16 val;
	val = (uint32)ustime;
	if (val < MAX_TIME) {
		histarea[val]++;
	}
	else {
		histarea[MAX_TIME]++;
	}
}
