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
 *  $Id: perf_prohook_t.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

#include "perf_prohook.h"
#include "perf_prohook_t.h"

/* overhead value */
volatile uint32 overhead_time = 0;

/* プロテクションフック呼び出し元判定用(ProHook_xx用) */
volatile uint8 call_prohook_obj = 0;

/* 割込み判定用フラグ */
volatile boolean check_boot_isr = FALSE;

/*
 *  実行時間分布を記録するメモリ領域（頻度: 65535回まで計測可能とする）
 */
uint16 histarea[4][MAX_TIME + 1];
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


/* *** ProtectionHook_A *** */
#ifdef USE_MC_KERNEL
	test_outputlog_0("== ProtectionHook_A[MC] ==");
#else  /* USE_MC_KERNEL */
	test_outputlog_0("== ProtectionHook_A ==");
#endif /* USE_MC_KERNEL */
	init_perf_func(4);

	for (i = 0; i < LOOP_COUNT; i++) {
		SEQUENCE_CHECK_START();
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		ActivateTask(NT_OSAP1_ProHook_A_Task1);
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 14);
	}
	test_outputlog_0("== ProHook-1 ==");
	print_hist(1);
	test_outputlog_0("== ProHook-2 ==");
	print_hist(3);
	test_outputlog_0("== ProHook-3 ==");
	print_hist(2);
	test_outputlog_0("== ProHook-4 ==");
	print_hist(4);


/* *** ProtectionHook_B *** */
	test_outputlog_0("== ProtectionHook_B ==");
	init_perf_func(1);

	/* ProHook-5(1)  */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP2_ProHook_B_Task2);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(2) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP4_ProHook_B_Task4);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(3) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP6_ProHook_B_Task6);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(4) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP8_ProHook_B_Task8);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(5) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP10_ProHook_B_Task10);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(6) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP12_ProHook_B_Task12);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(7) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP14_ProHook_B_Task14);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(8) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP16_ProHook_B_Task16);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(9) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP18_ProHook_B_Task18);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	/* ProHook-5(10) */
	SEQUENCE_CHECK_START();
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
	ActivateTask(NT_OSAP20_ProHook_B_Task20);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	test_outputlog_0("== ProHook-5 ==");
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
 * ********* 各処理単位の定義 **********
 * *************************************
 */
/* *** ProtectionHook_A *** */
/* T_OSAP1 */
ISR(T_OSAP1_ProHook_A_Isr2)
{
	test_clear_int_req(INTNO_PROHOOK_A);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);

	call_prohook_obj = PROHOOK_A_IGNORE;

	/* ProHook-2 */
	begin_measure(3);

	/* trigger cpu exception */
	raise_cpu_exception();

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 8);
}

StatusType TRUSTED_prohook_isr2_tfn(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams)
{
	test_int_raise(INTNO_PROHOOK_A);
	return(E_OK);
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
ProtectionReturnType
ProtectionHook(StatusType FatalError)
{
	StatusType ret_val;

	/* 呼び出し元判定 */
	switch(call_prohook_obj) {
		case PROHOOK_A_TERMINATETASKISR: /* NT_OSAP1_ProHook_A_Task2 */
			end_measure(1);

			ret_val = PRO_TERMINATETASKISR;
			SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
			/* ProHook-3 */
			begin_measure(2);
			break;
		case PROHOOK_A_IGNORE: /* T_OSAP1_ProHook_A_Isr2 */
			end_measure(3);

			ret_val = PRO_IGNORE;
			SEQUENCE_CHECK(OS_CORE_ID_MASTER, 7);
			break;
		case PROHOOK_A_RESTART: /* NT_OSAP2_ProHook_A_Task3 */
			SEQUENCE_CHECK(OS_CORE_ID_MASTER, 11);
			ret_val = PRO_TERMINATEAPPL_RESTART;

			/* ProHook-4 */
			begin_measure(4);
			break;
		case PROHOOK_B_TERMINATETASKISR: /* NT_OSAPxx_ProHook_B_Task2 */
			SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

			/* ProHook-5 */
			begin_measure(1);
			ret_val = PRO_TERMINATEAPPL;
			break;
		default:
			test_outputlog_0("ProtectionHook was occured!!");
			ret_val = PRO_SHUTDOWN;
			break;
	}
	return(ret_val);
}


/*
 *  エラーフックルーチン
 */
void ErrorHook(StatusType Error)
{
	test_outputlog_0("Error was occured!!");
}
