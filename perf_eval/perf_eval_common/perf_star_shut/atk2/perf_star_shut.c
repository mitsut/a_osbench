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
 *  $Id: perf_star_shut.c 653 2016-05-06 19:03:42Z ertl-honda $
 */


#include "perf_star_shut.h"


/*
 *  実行時間分布を記録するメモリ領域（頻度: 65535回まで計測可能とする）
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
	static uint32  cnt_main = 0;
	uint32 i;

	/* 2回目以降 */
	if (cnt_main != FIRST_COUNT) {
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 4);
#ifdef VALIDATE_SEQUENCE
		if (cnt_main < LOOP_COUNT) {
			SEQUENCE_CHECK_START();
			SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		}
#endif  /* VALIDATE_SEQUENCE */
	}
	/* 初回起動 */
	else {
		test_init_test_lib();
		perf_timer_initialize();

		test_init_check_cnt();

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

		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 1);
		init_hist(1, MAX_TIME, histarea[0]);   /* StartOS()用    */
	}

	/* 実行回数インクリメント */
	cnt_main++;

    /* StartOS()をLOOP_COUNT回行なう */
	if (cnt_main <= LOOP_COUNT) {
		SEQUENCE_CHECK(OS_CORE_ID_MASTER, 2);
		begin_measure(1);
		StartOS(MainApp);
	}

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 5);

	test_outputlog_0("== START_SHUTDOWN ==");
	test_outputlog_0("= Star-1 =");
	print_hist(1);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 6);
	test_outputlog_0("All evaluation items were measured.");

	while (1) {}
}   /* main */


/*
 * *************************************
 * *********** メインタスク ************
 * *************************************
 */
TASK(MainTask)
{
	end_measure(1);
	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	ShutdownOS(E_OK);
}

TASK(StartShut_Task1)
{
	TerminateTask();
}

#if defined(PERF_OBJECT_4) || defined(PERF_OBJECT_8) || defined(PERF_OBJECT_16)
TASK(StartShut_Task2)
{
	TerminateTask();
}

TASK(StartShut_Task3)
{
	TerminateTask();
}

TASK(StartShut_Task4)
{
	TerminateTask();
}

#if defined(PERF_OBJECT_8) || defined(PERF_OBJECT_16)
TASK(StartShut_Task5)
{
	TerminateTask();
}

TASK(StartShut_Task6)
{
	TerminateTask();
}

TASK(StartShut_Task7)
{
	TerminateTask();
}

TASK(StartShut_Task8)
{
	TerminateTask();
}

#ifdef PERF_OBJECT_16
TASK(StartShut_Task9)
{
	TerminateTask();
}

TASK(StartShut_Task10)
{
	TerminateTask();
}

TASK(StartShut_Task11)
{
	TerminateTask();
}

TASK(StartShut_Task12)
{
	TerminateTask();
}

TASK(StartShut_Task13)
{
	TerminateTask();
}

TASK(StartShut_Task14)
{
	TerminateTask();
}

TASK(StartShut_Task15)
{
	TerminateTask();
}

TASK(StartShut_Task16)
{
	TerminateTask();
}
#endif /* PERF_OBJECT_16 */
#endif /* PERF_OBJECT_8 || PERF_OBJECT_16 */
#endif /* PERF_OBJECT_4 || PERF_OBJECT_8 || PERF_OBJECT_16 */

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
	test_outputlog_0("Error was occured!!");
}
