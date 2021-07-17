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
 *  $Id: perf_general_nt.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

#include "perf_general.h"
#include "perf_general_nt.h"


/* タスク判別フラグ */
#ifdef USE_MC_KERNEL
volatile boolean terapp_second_act = FALSE;
#endif /* USE_MC_KERNEL */

/*
 * *************************************
 * ********* 各処理単位の定義 **********
 * *************************************
 */
/* *** TrustedFunction *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_Tfn_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* Tfn-1 */
	begin_measure(1);
	CallTrustedFunction(TFN_TFN_1, NULL_PTR);
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* Tfn-2 */
	begin_measure(2);
	CallTrustedFunction(TFN_TFN_2, NULL_PTR);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}


/* *** TerApp_C *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_TerApp_C_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP1_TerApp_C_Task3);

	TerminateTask();

}

TASK(NT_OSAP1_TerApp_C_ReTask2)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	TerminateTask();
}

TASK(NT_OSAP1_TerApp_C_Task3)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-3 */
	begin_measure(1);
	TerminateApplication(NT_OSAP1_TERAPP_C, RESTART);

	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_TerApp_C_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP2_TerApp_C_Task3);

	TerminateTask();

}

TASK(NT_OSAP2_TerApp_C_ReTask2)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_C_Task3)
{
	ActivateTask(NT_OSAP2_TerApp_C_Task4);
	ActivateTask(NT_OSAP2_TerApp_C_Task5);
	ActivateTask(NT_OSAP2_TerApp_C_Task6);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-3 */
	begin_measure(1);
	TerminateApplication(NT_OSAP2_TERAPP_C, RESTART);

	TerminateTask();
}

TASK(NT_OSAP2_TerApp_C_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_C_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_C_Task6)
{
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_TerApp_C_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP3_TerApp_C_Task3);

	TerminateTask();

}

TASK(NT_OSAP3_TerApp_C_ReTask2)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task3)
{
	ActivateTask(NT_OSAP3_TerApp_C_Task4);
	ActivateTask(NT_OSAP3_TerApp_C_Task5);
	ActivateTask(NT_OSAP3_TerApp_C_Task6);
	ActivateTask(NT_OSAP3_TerApp_C_Task7);
	ActivateTask(NT_OSAP3_TerApp_C_Task8);
	ActivateTask(NT_OSAP3_TerApp_C_Task9);
	ActivateTask(NT_OSAP3_TerApp_C_Task10);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-3 */
	begin_measure(1);
	TerminateApplication(NT_OSAP3_TERAPP_C, RESTART);

	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_C_Task10)
{
	TerminateTask();
}

/* NT_OSAP4 */
TASK(NT_OSAP4_TerApp_C_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP4_TerApp_C_Task3);

	TerminateTask();

}

TASK(NT_OSAP4_TerApp_C_ReTask2)
{
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task3)
{
	ActivateTask(NT_OSAP4_TerApp_C_Task4);
	ActivateTask(NT_OSAP4_TerApp_C_Task5);
	ActivateTask(NT_OSAP4_TerApp_C_Task6);
	ActivateTask(NT_OSAP4_TerApp_C_Task7);
	ActivateTask(NT_OSAP4_TerApp_C_Task8);
	ActivateTask(NT_OSAP4_TerApp_C_Task9);
	ActivateTask(NT_OSAP4_TerApp_C_Task10);
	ActivateTask(NT_OSAP4_TerApp_C_Task11);
	ActivateTask(NT_OSAP4_TerApp_C_Task12);
	ActivateTask(NT_OSAP4_TerApp_C_Task13);
	ActivateTask(NT_OSAP4_TerApp_C_Task14);
	ActivateTask(NT_OSAP4_TerApp_C_Task15);
	ActivateTask(NT_OSAP4_TerApp_C_Task16);
	ActivateTask(NT_OSAP4_TerApp_C_Task17);
	ActivateTask(NT_OSAP4_TerApp_C_Task18);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-3 */
	begin_measure(1);
	TerminateApplication(NT_OSAP4_TERAPP_C, RESTART);

	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task17)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_C_Task18)
{
	TerminateTask();
}


/* *** TerApp_D *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_TerApp_D_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_D_ReTask2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_TerApp_D_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_D_ReTask2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP2_TerApp_D_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_D_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_D_Task6)
{
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_TerApp_D_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_ReTask2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_D_Task10)
{
	TerminateTask();
}

/* NT_OSAP4 */
TASK(NT_OSAP4_TerApp_D_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_ReTask2)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task17)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_D_Task18)
{
	TerminateTask();
}


/* *** TerApp_E *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_TerApp_E_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_E_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_E_ReTask3)
{
	/* TerApp-5(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_TerApp_E_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_E_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_E_ReTask3)
{
	/* TerApp-5(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP2_TerApp_E_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_E_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_E_Task6)
{
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_TerApp_E_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_ReTask3)
{
	/* TerApp-5(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_E_Task10)
{
	TerminateTask();
}

/* NT_OSAP4 */
TASK(NT_OSAP4_TerApp_E_ResetTask)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task1)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_ReTask3)
{
	/* TerApp-5(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task17)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_E_Task18)
{
	TerminateTask();
}


/* *** TerApp_F *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_TerApp_F_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* trigger Isr2 */
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_TERAPP_ISR2_TFN1, NULL_PTR);
	while (check_isr_act == FALSE);

	TerminateTask();
}

TASK(NT_OSAP1_TerApp_F_ReTask2)
{
	/* TerApp-6(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_TerApp_F_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* trigger Isr2 */
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_TERAPP_ISR2_TFN2, NULL_PTR);
	while (check_isr_act == FALSE);

	TerminateTask();
}

TASK(NT_OSAP2_TerApp_F_ReTask2)
{
	/* TerApp-6(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP2_TerApp_F_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_F_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_F_Task5)
{
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_TerApp_F_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* trigger Isr2 */
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_TERAPP_ISR2_TFN3, NULL_PTR);
	while (check_isr_act == FALSE);

	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_ReTask2)
{
	/* TerApp-6(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_F_Task9)
{
	TerminateTask();
}

/* NT_OSAP4 */
TASK(NT_OSAP4_TerApp_F_Task1)
{
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	/* trigger Isr2 */
	check_isr_act = FALSE;
	CallTrustedFunction(TRIGGER_TERAPP_ISR2_TFN4, NULL_PTR);
	while (check_isr_act == FALSE);

	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_ReTask2)
{
	/* TerApp-6(end_measure) */
	end_measure(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_F_Task17)
{
	TerminateTask();
}


/* *** CheckTaskMemory *** */
/* NT_OSAP1 */
const uint32 check_data = 1;

TASK(NT_OSAP1_ChkMem_Task1)
{
	struct	func_param local;

	local.data_size = sizeof(check_data);
	local.data_addr = (uint32*)&check_data;

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	CallTrustedFunction(CHKMEM_TFN1, &local);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	TerminateTask();
}


/* MC KERNEL */
#ifdef USE_MC_KERNEL
/* *** TerApp[MC] *** */
/* NT_OSAP1_S */
TASK(NT_OSAP1_S_TerApp_MC_Task2)
{
	while (1) {
	}
	TerminateTask();
}

TASK(NT_OSAP1_S_TerApp_MC_ReTask3)
{
	if (terapp_second_act == FALSE) {
		end_measure(1);

		/* CheckPoint(1) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);
		terapp_second_act = TRUE;
	}
	else {
		/* CheckPoint(2) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
		terapp_second_act = FALSE;
	}
	AllowAccess();

	TerminateTask();
}

/* NT_OSAP2_S */
TASK(NT_OSAP2_S_TerApp_MC_Task2)
{
	ActivateTask(NT_OSAP2_S_TerApp_MC_Task4);
	ActivateTask(NT_OSAP2_S_TerApp_MC_Task5);
	ActivateTask(NT_OSAP2_S_TerApp_MC_Task6);

	while (1) {
	}
	TerminateTask();
}

TASK(NT_OSAP2_S_TerApp_MC_ReTask3)
{
	if (terapp_second_act == FALSE) {
		end_measure(1);

		/* CheckPoint(1) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);
		terapp_second_act = TRUE;
	}
	else {
		/* CheckPoint(2) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
		terapp_second_act = FALSE;
	}
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP2_S_TerApp_MC_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP2_S_TerApp_MC_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP2_S_TerApp_MC_Task6)
{
	TerminateTask();
}

/* NT_OSAP3_S */
TASK(NT_OSAP3_S_TerApp_MC_Task2)
{
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task4);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task5);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task6);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task7);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task8);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task9);
	ActivateTask(NT_OSAP3_S_TerApp_MC_Task10);

	while (1) {
	}
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_ReTask3)
{
	if (terapp_second_act == FALSE) {
		end_measure(1);

		/* CheckPoint(1) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);
		terapp_second_act = TRUE;
	}
	else {
		/* CheckPoint(2) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
		terapp_second_act = FALSE;
	}
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP3_S_TerApp_MC_Task10)
{
	TerminateTask();
}

/* NT_OSAP4_S */
TASK(NT_OSAP4_S_TerApp_MC_Task2)
{
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task4);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task5);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task6);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task7);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task8);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task9);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task10);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task11);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task12);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task13);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task14);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task15);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task16);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task17);
	ActivateTask(NT_OSAP4_S_TerApp_MC_Task18);

	while (1) {
	}
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_ReTask3)
{
	if (terapp_second_act == FALSE) {
		end_measure(1);

		/* CheckPoint(1) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 1);
		terapp_second_act = TRUE;
	}
	else {
		/* CheckPoint(2) - OS_CORE_ID_SLAVE */
		SEQUENCE_CHECK(OS_CORE_ID_SLAVE, 2);
		terapp_second_act = FALSE;
	}
	AllowAccess();

	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task17)
{
	TerminateTask();
}

TASK(NT_OSAP4_S_TerApp_MC_Task18)
{
	TerminateTask();
}
#endif /* USE_MC_KERNEL */
