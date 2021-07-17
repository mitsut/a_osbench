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
 *  $Id: perf_terapp_t.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

#include "perf_terapp.h"
#include "perf_terapp_t.h"


/* overhead value */
volatile uint32 overhead_time = 0;

/*
 *  実行時間分布を記録するメモリ領域（頻度: 65535回まで計測可能とする）
 */
uint16 histarea[2][MAX_TIME + 1];
uint16 histarea100us[MAX_TIME_100US + 1];

/*
 * メイン関数
 */
sint32
main(void)
{
#ifndef USE_MC_KERNEL
	test_init_test_lib();
	perf_timer_initialize();
	StartOS(MainApp);
#else   /* USE_MC_KERNEL */
	StatusType	ercd;
	CoreIdType	i;

	if (GetCoreID() == OS_CORE_ID_MASTER) {
		test_init_test_lib();
		test_init_test_lib_eachcore();
		for (i = 0; i < TotalNumberOfCores; i++) {
			if (i != OS_CORE_ID_MASTER) {
				StartCore(i, &ercd);
			}
		}
		StartOS(MainApp);
	}
	else {
		test_init_test_lib_eachcore();
		StartOS(DONOTCARE);
	}
#endif /* USE_MC_KERNEL */

	while (1) {}
}   /* main */


/*
 *  histareaをまとめて初期化する関数
 */
void init_hist_group(uint8 loop)
{
	uint8 i;
	for (i = 0; i < loop; i++) {
		init_hist(i + TMIN_HISTID, MAX_TIME, histarea[i]);
	}
}


/* 計測前に実施する初期化関数 */
void init_perf_func(uint8 hist_loop)
{
	init_hist_group(hist_loop);

	perf_timer_initialize();

#ifdef USE_MC_KERNEL
	test_init_sync_cnt();
#endif /* USE_MC_KERNEL */
}


/*
 * *************************************
 * *********** メインタスク ************
 * *************************************
 */
TASK(MainTask)
{
	uint32 i;

	perf_timer_initialize();

#if defined(USE_MC_KERNEL) && defined(SLAVE_IDLE_TASK)
	ActivateTask(S_IdleTask);
#endif /* USE_MC_KERNEL && SLAVE_IDLE_TASK */

/* *** measure overhead *** */
	test_outputlog_0("== measure overhead ==");
	init_hist(1, MAX_TIME_100US, histarea100us);
	for (i = 0; i < LOOP_COUNT; i++) {
		begin_measure(1);
		end_measure(1);
	}
	print_hist(1);


/* *** OverHead Time *** */
	overhead_time = calc_hist_value(1, RET_MIN_VALUE);
	test_outputlog_1("== overhead_time:%d[100nsec] ==",overhead_time);


/* *** TerApp_B *** */
#ifdef USE_MC_KERNEL
	test_outputlog_0("== TerApp_B[MC] ==");
#else  /* USE_MC_KERNEL */
	test_outputlog_0("== TerApp_B ==");
#endif /* USE_MC_KERNEL */
	init_perf_func(1);

	/* TerApp-2(1) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP1_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(2) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP2_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(3) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP3_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(4) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP4_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(5) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP5_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(6) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP6_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(7) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP7_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(8) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP8_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(9) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP9_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	/* TerApp-2(10) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(T_OSAP10_TerApp_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);

	test_outputlog_0("== TerApp-2(number_of_objects:16) ==");
	print_hist(1);
	test_outputlog_0("All evaluation items were measured.");

#ifndef USE_MC_KERNEL
	ShutdownOS(E_OK);
#else  /* USE_MC_KERNEL */
	ShutdownAllCores(E_OK);
#endif /* USE_MC_KERNEL */
}


/*
 * *************************************
 * ***** 各処理単位の定義ここから ******
 * *************************************
 */
/* T_OSAP1 */
TASK(T_OSAP1_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP1_TerApp_B_Task1);
	ActivateTask(NT_OSAP1_TerApp_B_Task3);
	ActivateTask(NT_OSAP1_TerApp_B_Task4);
	ActivateTask(NT_OSAP1_TerApp_B_Task5);
	ActivateTask(NT_OSAP1_TerApp_B_Task6);
	ActivateTask(NT_OSAP1_TerApp_B_Task7);
	ActivateTask(NT_OSAP1_TerApp_B_Task8);
	ActivateTask(NT_OSAP1_TerApp_B_Task9);
	ActivateTask(NT_OSAP1_TerApp_B_Task10);
	ActivateTask(NT_OSAP1_TerApp_B_Task11);
	ActivateTask(NT_OSAP1_TerApp_B_Task12);
	ActivateTask(NT_OSAP1_TerApp_B_Task13);
	ActivateTask(NT_OSAP1_TerApp_B_Task14);
	ActivateTask(NT_OSAP1_TerApp_B_Task15);
	ActivateTask(NT_OSAP1_TerApp_B_Task16);
	ActivateTask(NT_OSAP1_TerApp_B_Task17);

	/* TerApp-2(1) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP1, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP2 */
TASK(T_OSAP2_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP2_TerApp_B_Task1);
	ActivateTask(NT_OSAP2_TerApp_B_Task3);
	ActivateTask(NT_OSAP2_TerApp_B_Task4);
	ActivateTask(NT_OSAP2_TerApp_B_Task5);
	ActivateTask(NT_OSAP2_TerApp_B_Task6);
	ActivateTask(NT_OSAP2_TerApp_B_Task7);
	ActivateTask(NT_OSAP2_TerApp_B_Task8);
	ActivateTask(NT_OSAP2_TerApp_B_Task9);
	ActivateTask(NT_OSAP2_TerApp_B_Task10);
	ActivateTask(NT_OSAP2_TerApp_B_Task11);
	ActivateTask(NT_OSAP2_TerApp_B_Task12);
	ActivateTask(NT_OSAP2_TerApp_B_Task13);
	ActivateTask(NT_OSAP2_TerApp_B_Task14);
	ActivateTask(NT_OSAP2_TerApp_B_Task15);
	ActivateTask(NT_OSAP2_TerApp_B_Task16);
	ActivateTask(NT_OSAP2_TerApp_B_Task17);

	/* TerApp-2(2) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP2, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP3 */
TASK(T_OSAP3_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP3_TerApp_B_Task1);
	ActivateTask(NT_OSAP3_TerApp_B_Task3);
	ActivateTask(NT_OSAP3_TerApp_B_Task4);
	ActivateTask(NT_OSAP3_TerApp_B_Task5);
	ActivateTask(NT_OSAP3_TerApp_B_Task6);
	ActivateTask(NT_OSAP3_TerApp_B_Task7);
	ActivateTask(NT_OSAP3_TerApp_B_Task8);
	ActivateTask(NT_OSAP3_TerApp_B_Task9);
	ActivateTask(NT_OSAP3_TerApp_B_Task10);
	ActivateTask(NT_OSAP3_TerApp_B_Task11);
	ActivateTask(NT_OSAP3_TerApp_B_Task12);
	ActivateTask(NT_OSAP3_TerApp_B_Task13);
	ActivateTask(NT_OSAP3_TerApp_B_Task14);
	ActivateTask(NT_OSAP3_TerApp_B_Task15);
	ActivateTask(NT_OSAP3_TerApp_B_Task16);
	ActivateTask(NT_OSAP3_TerApp_B_Task17);

	/* TerApp-2(3) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP3, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP4 */
TASK(T_OSAP4_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP4_TerApp_B_Task1);
	ActivateTask(NT_OSAP4_TerApp_B_Task3);
	ActivateTask(NT_OSAP4_TerApp_B_Task4);
	ActivateTask(NT_OSAP4_TerApp_B_Task5);
	ActivateTask(NT_OSAP4_TerApp_B_Task6);
	ActivateTask(NT_OSAP4_TerApp_B_Task7);
	ActivateTask(NT_OSAP4_TerApp_B_Task8);
	ActivateTask(NT_OSAP4_TerApp_B_Task9);
	ActivateTask(NT_OSAP4_TerApp_B_Task10);
	ActivateTask(NT_OSAP4_TerApp_B_Task11);
	ActivateTask(NT_OSAP4_TerApp_B_Task12);
	ActivateTask(NT_OSAP4_TerApp_B_Task13);
	ActivateTask(NT_OSAP4_TerApp_B_Task14);
	ActivateTask(NT_OSAP4_TerApp_B_Task15);
	ActivateTask(NT_OSAP4_TerApp_B_Task16);
	ActivateTask(NT_OSAP4_TerApp_B_Task17);

	/* TerApp-2(4) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP4, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP5 */
TASK(T_OSAP5_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP5_TerApp_B_Task1);
	ActivateTask(NT_OSAP5_TerApp_B_Task3);
	ActivateTask(NT_OSAP5_TerApp_B_Task4);
	ActivateTask(NT_OSAP5_TerApp_B_Task5);
	ActivateTask(NT_OSAP5_TerApp_B_Task6);
	ActivateTask(NT_OSAP5_TerApp_B_Task7);
	ActivateTask(NT_OSAP5_TerApp_B_Task8);
	ActivateTask(NT_OSAP5_TerApp_B_Task9);
	ActivateTask(NT_OSAP5_TerApp_B_Task10);
	ActivateTask(NT_OSAP5_TerApp_B_Task11);
	ActivateTask(NT_OSAP5_TerApp_B_Task12);
	ActivateTask(NT_OSAP5_TerApp_B_Task13);
	ActivateTask(NT_OSAP5_TerApp_B_Task14);
	ActivateTask(NT_OSAP5_TerApp_B_Task15);
	ActivateTask(NT_OSAP5_TerApp_B_Task16);
	ActivateTask(NT_OSAP5_TerApp_B_Task17);

	/* TerApp-2(5) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP5, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP6 */
TASK(T_OSAP6_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP6_TerApp_B_Task1);
	ActivateTask(NT_OSAP6_TerApp_B_Task3);
	ActivateTask(NT_OSAP6_TerApp_B_Task4);
	ActivateTask(NT_OSAP6_TerApp_B_Task5);
	ActivateTask(NT_OSAP6_TerApp_B_Task6);
	ActivateTask(NT_OSAP6_TerApp_B_Task7);
	ActivateTask(NT_OSAP6_TerApp_B_Task8);
	ActivateTask(NT_OSAP6_TerApp_B_Task9);
	ActivateTask(NT_OSAP6_TerApp_B_Task10);
	ActivateTask(NT_OSAP6_TerApp_B_Task11);
	ActivateTask(NT_OSAP6_TerApp_B_Task12);
	ActivateTask(NT_OSAP6_TerApp_B_Task13);
	ActivateTask(NT_OSAP6_TerApp_B_Task14);
	ActivateTask(NT_OSAP6_TerApp_B_Task15);
	ActivateTask(NT_OSAP6_TerApp_B_Task16);
	ActivateTask(NT_OSAP6_TerApp_B_Task17);

	/* TerApp-2(6) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP6, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP7 */
TASK(T_OSAP7_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP7_TerApp_B_Task1);
	ActivateTask(NT_OSAP7_TerApp_B_Task3);
	ActivateTask(NT_OSAP7_TerApp_B_Task4);
	ActivateTask(NT_OSAP7_TerApp_B_Task5);
	ActivateTask(NT_OSAP7_TerApp_B_Task6);
	ActivateTask(NT_OSAP7_TerApp_B_Task7);
	ActivateTask(NT_OSAP7_TerApp_B_Task8);
	ActivateTask(NT_OSAP7_TerApp_B_Task9);
	ActivateTask(NT_OSAP7_TerApp_B_Task10);
	ActivateTask(NT_OSAP7_TerApp_B_Task11);
	ActivateTask(NT_OSAP7_TerApp_B_Task12);
	ActivateTask(NT_OSAP7_TerApp_B_Task13);
	ActivateTask(NT_OSAP7_TerApp_B_Task14);
	ActivateTask(NT_OSAP7_TerApp_B_Task15);
	ActivateTask(NT_OSAP7_TerApp_B_Task16);
	ActivateTask(NT_OSAP7_TerApp_B_Task17);

	/* TerApp-2(7) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP7, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP8 */
TASK(T_OSAP8_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP8_TerApp_B_Task1);
	ActivateTask(NT_OSAP8_TerApp_B_Task3);
	ActivateTask(NT_OSAP8_TerApp_B_Task4);
	ActivateTask(NT_OSAP8_TerApp_B_Task5);
	ActivateTask(NT_OSAP8_TerApp_B_Task6);
	ActivateTask(NT_OSAP8_TerApp_B_Task7);
	ActivateTask(NT_OSAP8_TerApp_B_Task8);
	ActivateTask(NT_OSAP8_TerApp_B_Task9);
	ActivateTask(NT_OSAP8_TerApp_B_Task10);
	ActivateTask(NT_OSAP8_TerApp_B_Task11);
	ActivateTask(NT_OSAP8_TerApp_B_Task12);
	ActivateTask(NT_OSAP8_TerApp_B_Task13);
	ActivateTask(NT_OSAP8_TerApp_B_Task14);
	ActivateTask(NT_OSAP8_TerApp_B_Task15);
	ActivateTask(NT_OSAP8_TerApp_B_Task16);
	ActivateTask(NT_OSAP8_TerApp_B_Task17);

	/* TerApp-2(8) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP8, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP9 */
TASK(T_OSAP9_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP9_TerApp_B_Task1);
	ActivateTask(NT_OSAP9_TerApp_B_Task3);
	ActivateTask(NT_OSAP9_TerApp_B_Task4);
	ActivateTask(NT_OSAP9_TerApp_B_Task5);
	ActivateTask(NT_OSAP9_TerApp_B_Task6);
	ActivateTask(NT_OSAP9_TerApp_B_Task7);
	ActivateTask(NT_OSAP9_TerApp_B_Task8);
	ActivateTask(NT_OSAP9_TerApp_B_Task9);
	ActivateTask(NT_OSAP9_TerApp_B_Task10);
	ActivateTask(NT_OSAP9_TerApp_B_Task11);
	ActivateTask(NT_OSAP9_TerApp_B_Task12);
	ActivateTask(NT_OSAP9_TerApp_B_Task13);
	ActivateTask(NT_OSAP9_TerApp_B_Task14);
	ActivateTask(NT_OSAP9_TerApp_B_Task15);
	ActivateTask(NT_OSAP9_TerApp_B_Task16);
	ActivateTask(NT_OSAP9_TerApp_B_Task17);

	/* TerApp-2(9) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP9, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* T_OSAP10 */
TASK(T_OSAP10_TerApp_B_Task2)
{
	ActivateTask(NT_OSAP10_TerApp_B_Task1);
	ActivateTask(NT_OSAP10_TerApp_B_Task3);
	ActivateTask(NT_OSAP10_TerApp_B_Task4);
	ActivateTask(NT_OSAP10_TerApp_B_Task5);
	ActivateTask(NT_OSAP10_TerApp_B_Task6);
	ActivateTask(NT_OSAP10_TerApp_B_Task7);
	ActivateTask(NT_OSAP10_TerApp_B_Task8);
	ActivateTask(NT_OSAP10_TerApp_B_Task9);
	ActivateTask(NT_OSAP10_TerApp_B_Task10);
	ActivateTask(NT_OSAP10_TerApp_B_Task11);
	ActivateTask(NT_OSAP10_TerApp_B_Task12);
	ActivateTask(NT_OSAP10_TerApp_B_Task13);
	ActivateTask(NT_OSAP10_TerApp_B_Task14);
	ActivateTask(NT_OSAP10_TerApp_B_Task15);
	ActivateTask(NT_OSAP10_TerApp_B_Task16);
	ActivateTask(NT_OSAP10_TerApp_B_Task17);

	/* TerApp-2(10) */
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
	begin_measure(1);
	TerminateApplication(NT_OSAP10, NO_RESTART);
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	TerminateTask();
}

/* MC KERNEL */
#if defined(USE_MC_KERNEL) && defined(SLAVE_IDLE_TASK)
/* IdleTask(SlaveCore) */
TASK(S_IdleTask)
{
	idle_task_process(OS_CORE_ID_SLAVE);
	TerminateTask();
}
#endif /* USE_MC_KERNEL && SLAVE_IDLE_TASK */


/*
 *  プロテクションフックルーチン
 */
#ifdef CFG_USE_PROTECTIONHOOK
ProtectionReturnType
ProtectionHook(StatusType FatalError)
{
	test_outputlog_0("ProtectionHook was occured!!");
	return PRO_SHUTDOWN;
}
#endif /* CFG_USE_PROTECTIONHOOK */


/*
 *  エラーフックルーチン
 */
void ErrorHook(StatusType Error)
{
	test_outputlog_0("Error was occured!!");
}
