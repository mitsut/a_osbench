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
 *  $Id: perf_general_main_task.c 646 2016-05-06 05:07:47Z ertl-honda $
 */


#include "perf_general.h"


/*
 *  実行時間分布を記録するメモリ領域
 */
uint16 histarea[2][MAX_TIME + 1];
uint16 histarea100us[MAX_TIME_100US + 1];

/* overhead value */
volatile uint32 overhead_time = 0;

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


/* *** TrustedFunction *** */
	test_outputlog_0("== TrustedFunction ==");
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_Tfn_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== Tfn-1 ==");
	print_hist(1);
	test_outputlog_0("== Tfn-2 ==");
	print_hist(2);


/* *** TerApp_C *** */
	test_outputlog_0("==  TerApp_C ==");
	/* number_of_objects:1 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_TerApp_C_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-3(number_of_objects:1) ==");
	print_hist(1);

	/* number_of_objects:4 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP2_TerApp_C_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-3(number_of_objects:4) ==");
	print_hist(1);

	/* number_of_objects:8 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP3_TerApp_C_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-3(number_of_objects:8) ==");
	print_hist(1);

	/* number_of_objects:16 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP4_TerApp_C_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-3(number_of_objects:16) ==");
	print_hist(1);


/* *** TerApp_D *** */
	test_outputlog_0("== TerApp_D ==");
	/* number_of_objects:1 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP1_TerApp_D_Task3);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-4(number_of_objects:1) ==");
	print_hist(1);

	/* number_of_objects:4 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_TerApp_D_Task3);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-4(number_of_objects:4) ==");
	print_hist(1);

	/* number_of_objects:8 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP3_TerApp_D_Task3);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-4(number_of_objects:8) ==");
	print_hist(1);

	/* number_of_objects:16 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP4_TerApp_D_Task3);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-4(number_of_objects:16) ==");
	print_hist(1);


/* *** TerApp_E *** */
	test_outputlog_0("== TerApp_E ==");
	/* number_of_objects:1 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP1_TerApp_E_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-5(number_of_objects:1) ==");
	print_hist(1);

	/* number_of_objects:4 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_TerApp_E_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-5(number_of_objects:4) ==");
	print_hist(1);

	/* number_of_objects:8 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP3_TerApp_E_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-5(number_of_objects:8) ==");
	print_hist(1);

	/* number_of_objects:16 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP4_TerApp_E_Task2);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	}
	test_outputlog_0("== TerApp-5(number_of_objects:16) ==");
	print_hist(1);


/* *** TerApp_F *** */
	test_outputlog_0("== TerApp_F ==");
	/* number_of_objects:1 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_TerApp_F_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("== TerApp-6(number_of_objects:1) ==");
	print_hist(1);

	/* number_of_objects:4 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP2_TerApp_F_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("== TerApp-6(number_of_objects:4) ==");
	print_hist(1);

	/* number_of_objects:8 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP3_TerApp_F_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("== TerApp-6(number_of_objects:8) ==");
	print_hist(1);

	/* number_of_objects:16 */
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP4_TerApp_F_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);
	}
	test_outputlog_0("== TerApp-6(number_of_objects:16) ==");
	print_hist(1);


#ifdef USE_MC_KERNEL
/* *** TerApp[MC] *** */
	test_outputlog_0("== TerApp[MC] ==");
	/* number_of_objects:1 */
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP1_M_TerApp_MC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}
	test_outputlog_0("== TerApp-7(number_of_objects:1) ==");
	print_hist(1);
	test_outputlog_0("== TerApp-8(number_of_objects:1) ==");
	print_hist(2);

	/* number_of_objects:4 */
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP2_M_TerApp_MC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}
	test_outputlog_0("== TerApp-7(number_of_objects:4) ==");
	print_hist(1);
	test_outputlog_0("== TerApp-8(number_of_objects:4) ==");
	print_hist(2);

	/* number_of_objects:8 */
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP3_M_TerApp_MC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}
	test_outputlog_0("== TerApp-7(number_of_objects:8) ==");
	print_hist(1);
	test_outputlog_0("== TerApp-8(number_of_objects:8) ==");
	print_hist(2);

	/* number_of_objects:16 */
	init_perf_func(2);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(T_OSAP4_M_TerApp_MC_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
	}
	test_outputlog_0("== TerApp-7(number_of_objects:16) ==");
	print_hist(1);
	test_outputlog_0("== TerApp-8(number_of_objects:16) ==");
	print_hist(2);
#endif /* USE_MC_KERNEL */


/* *** CheckTaskMemory *** */
	test_outputlog_0("== CheckTaskMemory ==");
	init_perf_func(1);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_ChkMem_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);	
	}
	test_outputlog_0("== ChkMem-1 ==");
	print_hist(1);

	test_outputlog_0("All evaluation items were measured.");

#ifndef USE_MC_KERNEL
	ShutdownOS(E_OK);
#else  /* USE_MC_KERNEL */
	ShutdownAllCores(E_OK);
#endif /* USE_MC_KERNEL */
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
