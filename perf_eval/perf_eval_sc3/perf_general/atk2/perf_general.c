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


/* 割込み判定用フラグ */
volatile boolean check_isr_act = FALSE;


/*
 * *************************************
 * ********* 各処理単位の定義 **********
 * *************************************
 */
/* *** TrustedFunction *** */
/* 信頼関数(Tfn_TFN_1) */
StatusType TRUSTED_tfn_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	return(E_OK);
}

/* 信頼関数(Tfn_TFN_2) */
StatusType TRUSTED_tfn_tfn2(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	end_measure(2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	return(E_OK);
}


/* *** TerApp_D *** */
/* T_OSAP1 */
TASK(T_OSAP1_TerApp_D_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP1_TerApp_D_Task1);

	/* TerApp-4 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP1_TERAPP_D, RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	TerminateTask();
}

/* T_OSAP2 */
TASK(T_OSAP2_TerApp_D_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP2_TerApp_D_Task1);
	ActivateTask(NT_OSAP2_TerApp_D_Task4);
	ActivateTask(NT_OSAP2_TerApp_D_Task5);
	ActivateTask(NT_OSAP2_TerApp_D_Task6);

	/* TerApp-4 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP2_TERAPP_D, RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	TerminateTask();
}

/* T_OSAP3 */
TASK(T_OSAP3_TerApp_D_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP3_TerApp_D_Task1);
	ActivateTask(NT_OSAP3_TerApp_D_Task4);
	ActivateTask(NT_OSAP3_TerApp_D_Task5);
	ActivateTask(NT_OSAP3_TerApp_D_Task6);
	ActivateTask(NT_OSAP3_TerApp_D_Task7);
	ActivateTask(NT_OSAP3_TerApp_D_Task8);
	ActivateTask(NT_OSAP3_TerApp_D_Task9);
	ActivateTask(NT_OSAP3_TerApp_D_Task10);

	/* TerApp-4 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP3_TERAPP_D, RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	TerminateTask();
}

/* T_OSAP4 */
TASK(T_OSAP4_TerApp_D_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP4_TerApp_D_Task1);
	ActivateTask(NT_OSAP4_TerApp_D_Task4);
	ActivateTask(NT_OSAP4_TerApp_D_Task5);
	ActivateTask(NT_OSAP4_TerApp_D_Task6);
	ActivateTask(NT_OSAP4_TerApp_D_Task7);
	ActivateTask(NT_OSAP4_TerApp_D_Task8);
	ActivateTask(NT_OSAP4_TerApp_D_Task9);
	ActivateTask(NT_OSAP4_TerApp_D_Task10);
	ActivateTask(NT_OSAP4_TerApp_D_Task11);
	ActivateTask(NT_OSAP4_TerApp_D_Task12);
	ActivateTask(NT_OSAP4_TerApp_D_Task13);
	ActivateTask(NT_OSAP4_TerApp_D_Task14);
	ActivateTask(NT_OSAP4_TerApp_D_Task15);
	ActivateTask(NT_OSAP4_TerApp_D_Task16);
	ActivateTask(NT_OSAP4_TerApp_D_Task17);
	ActivateTask(NT_OSAP4_TerApp_D_Task18);

	/* TerApp-4 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP4_TERAPP_D, RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	TerminateTask();
}


/* *** TerApp_E *** */
/* T_OSAP1 */
TASK(T_OSAP1_TerApp_E_Task2)
{
	/* ResetTask */
	ActivateTask(NT_OSAP1_TerApp_E_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP1_TerApp_E_Task1);

	/* TerApp-5(begin_measure) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP1_TERAPP_E, RESTART);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	TerminateTask();
}

/* T_OSAP2 */
TASK(T_OSAP2_TerApp_E_Task2)
{
	/* ResetTask */
	ActivateTask(NT_OSAP2_TerApp_E_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP2_TerApp_E_Task1);
	ActivateTask(NT_OSAP2_TerApp_E_Task4);
	ActivateTask(NT_OSAP2_TerApp_E_Task5);
	ActivateTask(NT_OSAP2_TerApp_E_Task6);

	/* TerApp-5(begin_measure) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP2_TERAPP_E, RESTART);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	TerminateTask();
}

/* T_OSAP3 */
TASK(T_OSAP3_TerApp_E_Task2)
{
	/* ResetTask */
	ActivateTask(NT_OSAP3_TerApp_E_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP3_TerApp_E_Task1);
	ActivateTask(NT_OSAP3_TerApp_E_Task4);
	ActivateTask(NT_OSAP3_TerApp_E_Task5);
	ActivateTask(NT_OSAP3_TerApp_E_Task6);
	ActivateTask(NT_OSAP3_TerApp_E_Task7);
	ActivateTask(NT_OSAP3_TerApp_E_Task8);
	ActivateTask(NT_OSAP3_TerApp_E_Task9);
	ActivateTask(NT_OSAP3_TerApp_E_Task10);

	/* TerApp-5(begin_measure) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP3_TERAPP_E, RESTART);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	TerminateTask();
}

/* T_OSAP4 */
TASK(T_OSAP4_TerApp_E_Task2)
{
	/* ResetTask */
	ActivateTask(NT_OSAP4_TerApp_E_ResetTask);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP4_TerApp_E_Task1);
	ActivateTask(NT_OSAP4_TerApp_E_Task4);
	ActivateTask(NT_OSAP4_TerApp_E_Task5);
	ActivateTask(NT_OSAP4_TerApp_E_Task6);
	ActivateTask(NT_OSAP4_TerApp_E_Task7);
	ActivateTask(NT_OSAP4_TerApp_E_Task8);
	ActivateTask(NT_OSAP4_TerApp_E_Task9);
	ActivateTask(NT_OSAP4_TerApp_E_Task10);
	ActivateTask(NT_OSAP4_TerApp_E_Task11);
	ActivateTask(NT_OSAP4_TerApp_E_Task12);
	ActivateTask(NT_OSAP4_TerApp_E_Task13);
	ActivateTask(NT_OSAP4_TerApp_E_Task14);
	ActivateTask(NT_OSAP4_TerApp_E_Task15);
	ActivateTask(NT_OSAP4_TerApp_E_Task16);
	ActivateTask(NT_OSAP4_TerApp_E_Task17);
	ActivateTask(NT_OSAP4_TerApp_E_Task18);

	/* TerApp-5(begin_measure) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP4_TERAPP_E, RESTART);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	TerminateTask();
}


/* *** TerApp_F *** */
/* T_OSAP1 */
ISR(T_OSAP1_TerApp_F_Isr2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/*  割込み判定用フラグクリア */
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_TERAPP_F1);

	/* TerApp-6(begin_measure) */
	begin_measure(1);
	TerminateApplication(NT_OSAP1_TERAPP_F, RESTART);
}

StatusType TRUSTED_terapp_isr2_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_TERAPP_F1);
	return(E_OK);
}

/* T_OSAP2 */
ISR(T_OSAP2_TerApp_F_Isr2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/*  割込み判定用フラグクリア */
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_TERAPP_F2);

	ActivateTask(NT_OSAP2_TerApp_F_Task3);
	ActivateTask(NT_OSAP2_TerApp_F_Task4);
	ActivateTask(NT_OSAP2_TerApp_F_Task5);

	/* TerApp-6(begin_measure) */
	begin_measure(1);
	TerminateApplication(NT_OSAP2_TERAPP_F, RESTART);
}

StatusType TRUSTED_terapp_isr2_tfn2(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_TERAPP_F2);
	return(E_OK);
}

/* T_OSAP3 */
ISR(T_OSAP3_TerApp_F_Isr2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/*  割込み判定用フラグクリア */
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_TERAPP_F3);

	ActivateTask(NT_OSAP3_TerApp_F_Task3);
	ActivateTask(NT_OSAP3_TerApp_F_Task4);
	ActivateTask(NT_OSAP3_TerApp_F_Task5);
	ActivateTask(NT_OSAP3_TerApp_F_Task6);
	ActivateTask(NT_OSAP3_TerApp_F_Task7);
	ActivateTask(NT_OSAP3_TerApp_F_Task8);
	ActivateTask(NT_OSAP3_TerApp_F_Task9);

	/* TerApp-6(begin_measure) */
	begin_measure(1);
	TerminateApplication(NT_OSAP3_TERAPP_F, RESTART);
}

StatusType TRUSTED_terapp_isr2_tfn3(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_TERAPP_F3);
	return(E_OK);
}

/* T_OSAP4 */
ISR(T_OSAP4_TerApp_F_Isr2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/*  割込み判定用フラグクリア */
	check_isr_act = TRUE;
	test_clear_int_req(INTNO_TERAPP_F4);

	ActivateTask(NT_OSAP4_TerApp_F_Task3);
	ActivateTask(NT_OSAP4_TerApp_F_Task4);
	ActivateTask(NT_OSAP4_TerApp_F_Task5);
	ActivateTask(NT_OSAP4_TerApp_F_Task6);
	ActivateTask(NT_OSAP4_TerApp_F_Task7);
	ActivateTask(NT_OSAP4_TerApp_F_Task8);
	ActivateTask(NT_OSAP4_TerApp_F_Task9);
	ActivateTask(NT_OSAP4_TerApp_F_Task10);
	ActivateTask(NT_OSAP4_TerApp_F_Task11);
	ActivateTask(NT_OSAP4_TerApp_F_Task12);
	ActivateTask(NT_OSAP4_TerApp_F_Task13);
	ActivateTask(NT_OSAP4_TerApp_F_Task14);
	ActivateTask(NT_OSAP4_TerApp_F_Task15);
	ActivateTask(NT_OSAP4_TerApp_F_Task16);
	ActivateTask(NT_OSAP4_TerApp_F_Task17);

	/* TerApp-6(begin_measure) */
	begin_measure(1);
	TerminateApplication(NT_OSAP4_TERAPP_F, RESTART);
}

StatusType TRUSTED_terapp_isr2_tfn4(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_TERAPP_F4);
	return(E_OK);
}


/* *** CheckTaskMemory *** */
StatusType TRUSTED_ChkMem_tfn1(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	struct func_param *local_param = FunctionParams;

	/* ChkMem-1 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	CheckTaskMemoryAccess(NT_OSAP1_ChkMem_Task1, local_param->data_addr, local_param->data_size);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	return(E_OK);
}


#ifdef USE_MC_KERNEL
/* *** TerApp[MC] *** */
/* T_OSAP1_M */
TASK(T_OSAP1_M_TerApp_MC_Task1)
{
	TaskStateType	TerApp_task2_state;
	TaskStateType	TerApp_Retask3_state;
	ApplicationStateType TerApp_nosap_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP1_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP1_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP1_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != RUNNING) && (TerApp_Retask3_state != SUSPENDED));

	/* TerApp-7 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP1_S_TERAPP_MC, RESTART);

	do {
		GetApplicationState(NT_OSAP1_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP1_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP1_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	ActivateTask(NT_OSAP1_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP1_S_TerApp_MC_Task2, &TerApp_task2_state);
	} while (TerApp_task2_state != RUNNING);

	/* TerApp-8 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	begin_measure(2);
	TerminateApplication(NT_OSAP1_S_TERAPP_MC, RESTART);
	end_measure(2);

	do {
		GetApplicationState(NT_OSAP1_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP1_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP1_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	TerminateTask();
}

/* T_OSAP2_M */
TASK(T_OSAP2_M_TerApp_MC_Task1)
{
	TaskStateType	TerApp_task2_state;
	TaskStateType	TerApp_Retask3_state;
	TaskStateType	TerApp_task6_state;
	ApplicationStateType TerApp_nosap_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP2_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP2_S_TerApp_MC_Task6, &TerApp_task6_state);
	} while (TerApp_task6_state != READY);

	/* TerApp-7 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP2_S_TERAPP_MC, RESTART);

	do {
		GetApplicationState(NT_OSAP2_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP2_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP2_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	ActivateTask(NT_OSAP2_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP2_S_TerApp_MC_Task6, &TerApp_task6_state);
	} while (TerApp_task6_state != READY);

	/* TerApp-8 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	begin_measure(2);
	TerminateApplication(NT_OSAP2_S_TERAPP_MC, RESTART);
	end_measure(2);

	do {
		GetApplicationState(NT_OSAP2_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP2_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP2_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	TerminateTask();
}

/* T_OSAP3_M */
TASK(T_OSAP3_M_TerApp_MC_Task1)
{
	TaskStateType	TerApp_task2_state;
	TaskStateType	TerApp_Retask3_state;
	TaskStateType	TerApp_task10_state;
	ApplicationStateType TerApp_nosap_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP3_S_TerApp_MC_Task10, &TerApp_task10_state);
	} while (TerApp_task10_state != READY);

	/* TerApp-7 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP3_S_TERAPP_MC, RESTART);

	do {
		GetApplicationState(NT_OSAP3_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP3_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP3_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP3_S_TerApp_MC_Task10, &TerApp_task10_state);
	} while (TerApp_task10_state != READY);

	/* TerApp-8 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	begin_measure(2);
	TerminateApplication(NT_OSAP3_S_TERAPP_MC, RESTART);
	end_measure(2);

	do {
		GetApplicationState(NT_OSAP3_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP3_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP3_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	TerminateTask();
}

/* T_OSAP4_M */
TASK(T_OSAP4_M_TerApp_MC_Task1)
{
	TaskStateType	TerApp_task2_state;
	TaskStateType	TerApp_Retask3_state;
	TaskStateType	TerApp_task18_state;
	ApplicationStateType TerApp_nosap_state;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP4_S_TerApp_MC_Task18, &TerApp_task18_state);
	} while (TerApp_task18_state != READY);

	/* TerApp-7 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);
	begin_measure(1);
	TerminateApplication(NT_OSAP4_S_TERAPP_MC, RESTART);

	do {
		GetApplicationState(NT_OSAP4_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP4_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP4_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task2);

	do {
		GetTaskState(NT_OSAP4_S_TerApp_MC_Task18, &TerApp_task18_state);
	} while (TerApp_task18_state != READY);

	/* TerApp-8 */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	begin_measure(2);
	TerminateApplication(NT_OSAP4_S_TERAPP_MC, RESTART);
	end_measure(2);

	do {
		GetApplicationState(NT_OSAP4_S_TERAPP_MC, &TerApp_nosap_state);
	} while (TerApp_nosap_state != APPLICATION_ACCESSIBLE);

	do {
		GetTaskState(NT_OSAP4_S_TerApp_MC_Task2, &TerApp_task2_state);
		GetTaskState(NT_OSAP4_S_TerApp_MC_ReTask3, &TerApp_Retask3_state);
	} while ((TerApp_task2_state != SUSPENDED) && (TerApp_Retask3_state != SUSPENDED));

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
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
