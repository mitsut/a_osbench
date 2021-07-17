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
 *  $Id: perf_general_main_task.c 653 2016-05-06 19:03:42Z ertl-honda $
 */


#include "perf_general.h"


/*
 *  実行時間分布を記録するメモリ領域
 */
uint16 histarea[7][MAX_TIME + 1];
uint16 histarea100us[MAX_TIME_100US + 1];

/* overhead value */
volatile uint32 overhead_time = 0;

/* 割込み応答評価用の変数 */
uint32 cnt_int   = 0;
volatile boolean int_measure_end  = FALSE;
volatile INT_CATEGORY int_category = NONE;


/*
 * メイン関数
 */
sint32
main(void)
{
#ifndef USE_MC_KERNEL
	test_init_test_lib();
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
	volatile uint32 j, loop;

	perf_timer_initialize();

/* *** Act IdleTask(Slave) *** */
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


/* *** 100us test *** */
	test_outputlog_0("== 100us test ==");
	init_hist(1, MAX_TIME_100US, histarea100us);
	loop = NO_LOOP1;
	for (i = 0; i < LOOP_COUNT; i++) {
		begin_measure(1);
		for (j = 0; j < loop; j++) {
		}
		end_measure(1);
	}
	print_hist(1);


#if 0
/* *** 10s test *** */
	test_outputlog_0("== 10s test start ==");
	loop = NO_LOOP1 * 100000;
	for (j = 0; j < loop; j++) {
	}
	test_outputlog_0("== 10s test end ==");

#ifndef USE_MC_KERNEL
	ShutdownOS(E_OK);
#else  /* USE_MC_KERNEL */
	ShutdownAllCores(E_OK);
#endif /* USE_MC_KERNEL */
#endif /* 0 */


/* *** ActivateTask *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== ActivateTask(T_OSAP1 -> T_OSAP1) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== ActivateTask ==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(Act_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
	}
	test_outputlog_0("= Act-1 =");
	print_hist(1);
	test_outputlog_0("= Act-2 =");
	print_hist(2);
	test_outputlog_0("= Act-3 =");
	print_hist(3);
	test_outputlog_0("= Act-4 =");
	print_hist(4);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP -> NT_OSAP(同一) */
	test_outputlog_0("== ActivateTask(NT_OSAP1 -> NT_OSAP1) ==");
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_Act_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}
	test_outputlog_0("= Act-1 =");
	print_hist(1);
	test_outputlog_0("= Act-2 =");
	print_hist(2);
	/* Act-3,Act-4はISR2がT_OSAPのため測定対象外 */

	/* NT_OSAP1 -> T_OSAP -> NT_OSAP1 */
	test_outputlog_0("== ActivateTask(NT_OSAP1 -> T_OSAP2 -> NT_OSAP1) ==");
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_Act_Task2_tnt1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
	}
	test_outputlog_0("= Act-1 =");
	print_hist(1);
	test_outputlog_0("= Act-2 =");
	print_hist(2);
	test_outputlog_0("= Act-3 =");
	print_hist(3);
	test_outputlog_0("= Act-4 =");
	print_hist(4);

	/* NT_OSAP2 -> T_OSAP -> NT_OSAP1 */
	test_outputlog_0("== ActivateTask(NT_OSAP2 -> T_OSAP2 -> NT_OSAP1) ==");
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_Act_Task2_tnt2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
	}
	test_outputlog_0("= Act-1 =");
	print_hist(1);
	test_outputlog_0("= Act-2 =");
	print_hist(2);
	test_outputlog_0("= Act-3 =");
	print_hist(3);
	test_outputlog_0("= Act-4 =");
	print_hist(4);

	/* NT_OSAP -> NT_OSAP(別) */
	test_outputlog_0("== ActivateTask(NT_OSAP3 -> NT_OSAP2) ==");
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP3_Act_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}
	test_outputlog_0("= Act-1 =");
	print_hist(1);
	test_outputlog_0("= Act-2 =");
	print_hist(2);
	/* Act-3,Act-4はISR2がT_OSAPのため測定対象外 */
#endif /* USE_SCALABILITYCLASS3 */

#ifdef USE_MC_KERNEL
/* *** ActivateTask[MC] *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== ActivateTask[MC](T_OSAP1_M -> T_OSAP2_S) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== ActivateTask[MC] ==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(OSAP1_M_Act_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}

	test_outputlog_0("= Act-5 =");
	print_hist(1);
	test_outputlog_0("= Act-6 =");
	print_hist(3);
	test_outputlog_0("= Act-7 =");
	print_hist(2);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP1_M → NT_OSAP2_S */
	test_outputlog_0("== ActivateTask[MC](NT_OSAP1_M -> NT_OSAP2_S) ==");
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_M_Act_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}

	test_outputlog_0("= Act-5 =");
	print_hist(1);
	test_outputlog_0("= Act-6 =");
	print_hist(3);
	test_outputlog_0("= Act-7 =");
	print_hist(2);
#endif /* USE_SCALABILITYCLASS3 */
#endif /* USE_MC_KERNEL */


/* *** TerminateTask *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== TerminateTask(T_OSAP1 -> T_OSAP1) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== TerminateTask ==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(Ter_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("= Ter-1 =");
	print_hist(1);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP -> NT_OSAP(同一) */
	test_outputlog_0("== TerminateTask(NT_OSAP1 -> NT_OSAP1) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_Ter_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("= Ter-1 =");
	print_hist(1);

	/* NT_OSAP1 -> T_OSAP -> NT_OSAP1 */
	test_outputlog_0("== TerminateTask(NT_OSAP1 -> T_OSAP -> NT_OSAP1) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_Ter_Task2_1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("= Ter-1 =");
	print_hist(1);


	/* NT_OSAP2 -> T_OSAP -> NT_OSAP1 */
	test_outputlog_0("== TerminateTask(NT_OSAP2 -> T_OSAP -> NT_OSAP1) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_Ter_Task2_2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("= Ter-1 =");
	print_hist(1);

	/* NT_OSAP -> NT_OSAP(別) */
	test_outputlog_0("== TerminateTask(NT_OSAP2 -> NT_OSAP3) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP2_Ter_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("= Ter-1 =");
	print_hist(1);
#endif /* USE_SCALABILITYCLASS3 */


/* *** ChainTask *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== ChainTask(T_OSAP1 -> T_OSAP1) ==");
#else   /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== ChainTask ==");
#endif  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("= Chain-1 =");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(Chain_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	}
	print_hist(1);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP -> NT_OSAP(同一) */
	test_outputlog_0("== ChainTask(NT_OSAP1 -> NT_OSAP1) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_Chain_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	}
	test_outputlog_0("= Chain-1 =");
	print_hist(1);

	/* NT_OSAP1 -> T_OSAP -> NT_OSAP1 */
	test_outputlog_0("== ChainTask(NT_OSAP1 -> T_OSAP -> NT_OSAP1) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_Chain_Task1_1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	}
	test_outputlog_0("= Chain-1 =");
	print_hist(1);

	/* NT_OSAP2 -> T_OSAP -> NT_OSAP1 */
	test_outputlog_0("== ChainTask(NT_OSAP2 -> T_OSAP -> NT_OSAP1) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_Chain_Task1_2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	}
	test_outputlog_0("= Chain-1 =");
	print_hist(1);

	/* NT_OSAP -> NT_OSAP(別) */
	test_outputlog_0("== ChainTask(NT_OSAP2 -> NT_OSAP3) ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP2_Chain_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
	}
	test_outputlog_0("= Chain-1 =");
	print_hist(1);
#endif /* USE_SCALABILITYCLASS3 */


/* *** Interrupt *** */
	/* T_OSAP -> T_OSAP(同一) */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== Interrupt(T_OSAP) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Interrupt ==");
#endif  /* USE_SCALABILITYCLASS3 */
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(Interrupt_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("= Dis-1 =");
	print_hist(1);
	test_outputlog_0("= Ena-1 =");
	print_hist(2);
	test_outputlog_0("= Sus-1 =");
	print_hist(3);	
	test_outputlog_0("= Res-1 =");
	print_hist(4);

/* *** Resource *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== Resource(T_OSAP) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Resource ==");
#endif  /* USE_SCALABILITYCLASS3 */
	init_perf_func(6);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(Resource_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 13);
	}
	test_outputlog_0("= GetR-1 =");
	print_hist(1);
	test_outputlog_0("= Rel-11 =");
	print_hist(2);
	test_outputlog_0("= Rel-12 =");
	print_hist(3);	
	test_outputlog_0("= GetR-2 =");
	print_hist(4);
	test_outputlog_0("= Rel-21 =");
	print_hist(5);
	test_outputlog_0("= Rel-22 =");
	print_hist(6);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP -> NT_OSAP(同一) */
	test_outputlog_0("== Resource(NT_OSAP) ==");
	init_perf_func(6);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_Resource_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 13);
	}
	test_outputlog_0("= GetR-1 =");
	print_hist(1);
	test_outputlog_0("= Rel-11 =");
	print_hist(2);
	test_outputlog_0("= Rel-12 =");
	print_hist(3);	
	test_outputlog_0("= GetR-2 =");
	print_hist(4);
	test_outputlog_0("= Rel-21 =");
	print_hist(5);
	test_outputlog_0("= Rel-22 =");
	print_hist(6);
#endif /* USE_SCALABILITYCLASS3 */


/* *** Event *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== Event(T_OSAP) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Event ==");
#endif  /* USE_SCALABILITYCLASS3 */
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(Event_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	}

	test_outputlog_0("= WaitE-1 =");
	print_hist(3);
	test_outputlog_0("= WaitE-2 =");
	print_hist(1);
	test_outputlog_0("= GetE-1 =");
	print_hist(2);
	test_outputlog_0("= ClearE-1 =");
	print_hist(4);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP -> NT_OSAP(同一) */
	test_outputlog_0("== Event(NT_OSAP) ==");
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_Event_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	}

	test_outputlog_0("= WaitE-1 =");
	print_hist(3);
	test_outputlog_0("= WaitE-2 =");
	print_hist(1);
	test_outputlog_0("= GetE-1 =");
	print_hist(2);
	test_outputlog_0("= ClearE-1 =");
	print_hist(4);
#endif /* USE_SCALABILITYCLASS3 */


/* *** SetEvent *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== SetEvent(T_OSAP) ==");
#else   /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== SetEvent ==");
#endif  /* USE_SCALABILITYCLASS3 */
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(SetE_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 14);
	}
	test_outputlog_0("= SetE-1 =");
	print_hist(2);
	test_outputlog_0("= SetE-2 =");
	print_hist(1);
	test_outputlog_0("= SetE-3 =");
	print_hist(3);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP -> NT_OSAP(同一) */
	test_outputlog_0("== SetEvent(NT_OSAP) ==");
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_SetE_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 14);
	}
	test_outputlog_0("= SetE-1 =");
	print_hist(2);
	test_outputlog_0("= SetE-2 =");
	print_hist(1);
	test_outputlog_0("= SetE-3 =");
	print_hist(3);
#endif /* USE_SCALABILITYCLASS3 */


#ifdef USE_MC_KERNEL
/* *** SetEvent[MC] *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== SetEvent[MC](T_OSAP1_M -> T_OSAP2_S) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== SetEvent[MC]==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(OSAP1_M_SetE_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}

	test_outputlog_0("= SetE-4 =");
	print_hist(3);
	test_outputlog_0("= SetE-5 =");
	print_hist(2);
	test_outputlog_0("= SetE-6 =");
	print_hist(1);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP1_M → NT_OSAP2_S */
	test_outputlog_0("== SetEvent[MC](NT_OSAP1_M -> NT_OSAP2_S) ==");
	init_perf_func(3);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_M_SetE_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}

	test_outputlog_0("= SetE-4 =");
	print_hist(3);
	test_outputlog_0("= SetE-5 =");
	print_hist(2);
	test_outputlog_0("= SetE-6 =");
	print_hist(1);
#endif /* USE_SCALABILITYCLASS3 */


/* *** Spinlock[MC] *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== Spinlock(T_OSAP1_M) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== Spinlock[MC] ==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(OSAP1_M_Spin_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}

	test_outputlog_0("= GetS-1 =");
	print_hist(1);
	test_outputlog_0("= RelS-1 =");
	print_hist(2);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP1_M */
	test_outputlog_0("== Spinlock(NT_OSAP1_M) ==");
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_M_Spin_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}

	test_outputlog_0("= GetS-1 =");
	print_hist(1);
	test_outputlog_0("= RelS-1 =");
	print_hist(2);
#endif /* USE_SCALABILITYCLASS3 */
#endif /* USE_MC_KERNEL */


#if defined(USE_MC_KERNEL) || defined(USE_SCALABILITYCLASS3)
/* *** IOC *** */
#if USE_SCALABILITYCLASS3
	test_outputlog_0("== IOC(T_OSAP1 -> T_OSAP2) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== IOC ==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(5);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(OSAP1_IOC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	}
	test_outputlog_0("== IocS-1 ==");
	print_hist(1);
	test_outputlog_0("== IocS-2 ==");
	print_hist(2);
	test_outputlog_0("== IocR-1 ==");
	print_hist(3);
	test_outputlog_0("== IocR-2 ==");
	print_hist(4);
	test_outputlog_0("== IocR-3 ==");
	print_hist(5);
#endif /* USE_MC_KERNEL || USE_SCALABILITYCLASS3 */

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP1 -> NT_OSAP2 */
	test_outputlog_0("== IOC(NT_OSAP1 -> NT_OSAP2) ==");
	init_perf_func(5);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_IOC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 10);
	}
	test_outputlog_0("== IocS-1 ==");
	print_hist(1);
	test_outputlog_0("== IocS-2 ==");
	print_hist(2);
	test_outputlog_0("== IocR-1 ==");
	print_hist(3);
	test_outputlog_0("== IocR-2 ==");
	print_hist(4);
	test_outputlog_0("== IocR-3 ==");
	print_hist(5);
#endif /* USE_SCALABILITYCLASS3 */

#ifdef USE_MC_KERNEL
/* *** IOC[MC] *** */
#ifdef USE_SCALABILITYCLASS3
	test_outputlog_0("== IOC[MC](T_OSAP1_M -> T_OSAP2_S) ==");
#else  /* USE_SCALABILITYCLASS3 */
	test_outputlog_0("== IOC[MC] ==");
#endif /* USE_SCALABILITYCLASS3 */
	init_perf_func(5);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(OSAP1_M_IOC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== IocS-3 ==");
	print_hist(1);
	test_outputlog_0("== IocS-4 ==");
	print_hist(2);
	test_outputlog_0("== IocR-4 ==");
	print_hist(3);
	test_outputlog_0("== IocR-5 ==");
	print_hist(4);
	test_outputlog_0("== IocR-6 ==");
	print_hist(5);

#ifdef USE_SCALABILITYCLASS3
	/* NT_OSAP1_M → NT_OSAP2_S */
	test_outputlog_0("== IOC[MC](NT_OSAP1_M -> NT_OSAP2_S) ==");
	init_perf_func(5);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_M_IOC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== IocS-3 ==");
	print_hist(1);
	test_outputlog_0("== IocS-4 ==");
	print_hist(2);
	test_outputlog_0("== IocR-4 ==");
	print_hist(3);
	test_outputlog_0("== IocR-5 ==");
	print_hist(4);
	test_outputlog_0("== IocR-6 ==");
	print_hist(5);
#endif /* USE_SCALABILITYCLASS3 */
#endif /* USE_MC_KERNEL */


/* *** Interruption Response Performance Test *** */
	init_perf_func(7);

#ifndef USE_SCALABILITYCLASS3
#ifndef USE_MC_KERNEL
	i = (uint32)(3 * LOOP_COUNT / 1000);
#else  /* USE_MC_KERNEL */
	i = (uint32)(4 * LOOP_COUNT / 1000);
#endif /* USE_MC_KERNEL */

#else  /* USE_SCALABILITYCLASS3 */
#ifndef USE_MC_KERNEL
	i = (uint32)(5 * LOOP_COUNT / 1000);
#else  /* USE_MC_KERNEL */
	i = (uint32)(7 * LOOP_COUNT / 1000);
#endif /* USE_MC_KERNEL */
#endif /* USE_SCALABILITYCLASS3 */

	test_outputlog_1("== ISR2 Please wait for about %d[sec]... ==", i);

/* *** Int-1 *** */
	/* T_OSAP1 */
	ActivateTask(Int_A_Task1);

	/* NT_OSAP1 */
#ifdef USE_SCALABILITYCLASS3
	ActivateTask(NT_OSAP1_Int_A_Task1);
#endif /* USE_SCALABILITYCLASS3 */


/* *** Int-3 *** */
	/* T_OSAP1 */
	ActivateTask(Int_B_Task1);

	/* NT_OSAP1 */
#ifdef USE_SCALABILITYCLASS3
	ActivateTask(NT_OSAP1_Int_B_Task1);
#endif /* USE_SCALABILITYCLASS3 */


#ifdef USE_MC_KERNEL
/* *** Int-4 *** */
	/* T_OSAP1_M */
	ActivateTask(OSAP1_M_Int_MC_Task1);

	/* NT_OSAP1_M */
#ifdef USE_SCALABILITYCLASS3
	ActivateTask(NT_OSAP1_M_Int_MC_Task1);
#endif /* USE_SCALABILITYCLASS3 */
#endif  /* USE_MC_KERNEL */


/* *** Int-2 *** */
	int_category = INT_2;
	int_measure_end = FALSE;
	cnt_int = 0;

	int_timer_initialize(INT_A_TIMER);

	TerminateTask();
}


/*
 *  プロテクションフックルーチン
 */
ProtectionReturnType
ProtectionHook(StatusType FatalError)
{
	test_outputlog_0("ProtectionHook was occured!!");

	return(PRO_SHUTDOWN);
}


/*
 *  エラーフックルーチン
 */
void ErrorHook(StatusType Error)
{
}
