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
 *  $Id: perf_prohook_nt.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

#include "perf_prohook.h"
#include "perf_prohook_nt.h"


/*
 * *************************************
 * ***** 各処理単位の定義ここから ******
 * *************************************
 */
/* *** ProtectionHook_A *** */
/* NT_OSAP1 */
TASK(NT_OSAP1_ProHook_A_Task1)
{
	/* check point(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	ActivateTask(NT_OSAP1_ProHook_A_Task2);
	end_measure(2);

	/* check point(5) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* trigger Isr2 */
	CallTrustedFunction(PROHOOK_ISR2_TFN, NULL);

	/* check point(9) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 9);

	ActivateTask(NT_OSAP2_ProHook_A_Task3);

	/* check point(13) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 13);

	TerminateTask();
}

TASK(NT_OSAP1_ProHook_A_Task2)
{
	/* check point(3) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	call_prohook_obj = PROHOOK_A_TERMINATETASKISR;

	/* ProHook-1 */
	begin_measure(1);
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_ProHook_A_Task3)
{
	/* check point(10) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);

	call_prohook_obj = PROHOOK_A_RESTART;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

TASK(NT_OSAP2_ProHook_A_ReTask4)
{
	end_measure(4);

	/* check point(12) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 12);

	/* OSAP再起動 */
	AllowAccess();

	TerminateTask();
}


/* *** ProtectionHook_B *** */
/* NT_OSAP1 - ProHook-5(1) */
TASK(NT_OSAP1_ProHook_B_Task1)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP2 - ProHook-5(1) */
TASK(NT_OSAP2_ProHook_B_Task2)
{
	ActivateTask(NT_OSAP1_ProHook_B_Task1);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP3 - ProHook-5(2) */
TASK(NT_OSAP3_ProHook_B_Task3)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP4 - ProHook-5(2) */
TASK(NT_OSAP4_ProHook_B_Task4)
{
	ActivateTask(NT_OSAP3_ProHook_B_Task3);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP5 - ProHook-5(3) */
TASK(NT_OSAP5_ProHook_B_Task5)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP6 - ProHook-5(3) */
TASK(NT_OSAP6_ProHook_B_Task6)
{
	ActivateTask(NT_OSAP5_ProHook_B_Task5);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP7 - ProHook-5(4) */
TASK(NT_OSAP7_ProHook_B_Task7)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP8 - ProHook-5(4) */
TASK(NT_OSAP8_ProHook_B_Task8)
{
	ActivateTask(NT_OSAP7_ProHook_B_Task7);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP9 - ProHook-5(5) */
TASK(NT_OSAP9_ProHook_B_Task9)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP10 - ProHook-5(5) */
TASK(NT_OSAP10_ProHook_B_Task10)
{
	ActivateTask(NT_OSAP9_ProHook_B_Task9);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP11 - ProHook-5(6) */
TASK(NT_OSAP11_ProHook_B_Task11)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP12 - ProHook-5(6) */
TASK(NT_OSAP12_ProHook_B_Task12)
{
	ActivateTask(NT_OSAP11_ProHook_B_Task11);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP13 - ProHook-5(7) */
TASK(NT_OSAP13_ProHook_B_Task13)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP14 - ProHook-5(7) */
TASK(NT_OSAP14_ProHook_B_Task14)
{
	ActivateTask(NT_OSAP13_ProHook_B_Task13);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP15 - ProHook-5(8) */
TASK(NT_OSAP15_ProHook_B_Task15)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP16 - ProHook-5(8) */
TASK(NT_OSAP16_ProHook_B_Task16)
{
	ActivateTask(NT_OSAP15_ProHook_B_Task15);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP17 - ProHook-5(9) */
TASK(NT_OSAP17_ProHook_B_Task17)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP18 - ProHook-5(9) */
TASK(NT_OSAP18_ProHook_B_Task18)
{
	ActivateTask(NT_OSAP17_ProHook_B_Task17);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}

/* NT_OSAP19 - ProHook-5(10) */
TASK(NT_OSAP19_ProHook_B_Task19)
{
	end_measure(1);

	/* SEQUENCE_CHECK(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	TerminateTask();
}

/* NT_OSAP20 - ProHook-5(10) */
TASK(NT_OSAP20_ProHook_B_Task20)
{
	ActivateTask(NT_OSAP19_ProHook_B_Task19);

	/* SEQUENCE_CHECK(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);

	call_prohook_obj = PROHOOK_B_TERMINATETASKISR;

	/* trigger cpu exception */
	raise_cpu_exception();

	TerminateTask();
}
