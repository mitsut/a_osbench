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
 *  $Id: test_common_lib.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *  テストライブラリ(非依存部)
 */


#include "test_common_lib.h"


#ifndef TotalNumberOfCores
/* シングルコアの場合 */
static volatile uint32  check_count;
#else
/* マルチコアの場合 */
static volatile uint32  check_count_mc[TotalNumberOfCores];
static volatile uint32  local_phase[TotalNumberOfCores];
static volatile uint32  global_phase;

#define TEST_BUSY_LOOP   1000
#define TEST_LOOP_COUNT  100000
#endif  /* TotalNumberOfCores */

/*
 *  チェックポイント通過の状態(TRUE:正常，FALSE:異常)
 */
volatile uint8	cp_state;


/*
 * チェックポイントカウンタ初期化
 */
void
test_init_check_cnt(void)
{
#ifndef TotalNumberOfCores
	check_count = 0U;
#else
	/* マルチコア処理 */
	uint16 i;

	for (i = 0; i < TotalNumberOfCores; i++) {
		check_count_mc[i] = 0U;
	}
#endif
}


#ifdef TotalNumberOfCores
/*
 *  同期フェーズの初期化
 */
void
test_init_sync_cnt(void)
{
	uint16 i;

	for (i = 0; i < TotalNumberOfCores; i++) {
		local_phase[i] = 0U;
	}
	global_phase = 0U;
}
#endif


/*
 *  テストライブラリ初期化
 */
void
test_init_test_lib(void)
{
#ifndef TotalNumberOfCores
	check_count = 0U;
#else
	/* マルチコア処理 */
	uint16 i;

	for (i = 0; i < TotalNumberOfCores; i++) {
		check_count_mc[i] = 0U;
		local_phase[i] = 0U;
	}
	global_phase = 0U;
#endif /* TotalNumberOfCores */

	cp_state = TRUE;

	test_init_impl_lib();
}


#ifdef TotalNumberOfCores
void
test_init_test_lib_eachcore(void)
{
	test_init_impl_lib_eachcore();
}
#endif /* TotalNumberOfCores */


#ifndef TotalNumberOfCores
/*
 *  チェックポイント(シングルコア用)
 */
void
test_check_point(uint32 count)
{
	uint8	errorflag = FALSE;
	boolean	locked;

	/*
	 *  全割込み禁止状態でない場合，
	 *  SuspendAllInterruptsによる割込み禁止状態にする
	 */
	locked = test_dis_int();

	/*  シーケンスチェック */
	check_count++;
	if (check_count == count) {
		test_outputlog_1("Check point %d passed.", count);
	}
	else {
		test_outputlog_1("## Unexpected check point %d.", count);
		errorflag = TRUE;
	}

	/* 割込み禁止状態を解除 */
	test_ena_int(locked);

	if (errorflag == TRUE) {
		cp_state = FALSE;
		test_shutdown();
	}
}


/*
 *  完了チェックポイント(シングルコア用)
 */
void
test_check_finish(uint32 count)
{
	test_check_point(count);
	if (cp_state == TRUE) {
		test_outputlog_0("All check points passed.");
	}
	test_shutdown();
}

#else /* TotalNumberOfCores */
/*
 *  チェックポイント(マルチコア用)
 */
void
test_mc_check_point(uint8 id, uint32 count)
{
	uint8		errorflag = FALSE;
	boolean		locked;
	uint8		coreid;

	/*
	 *  全割込み禁止状態でない場合，
	 *  SuspendAllInterruptsによる割込み禁止状態にする
	 */
	locked = test_dis_int();

	/* コアID取得 */
	coreid = GetCoreID();

	/*
	 * 引数で指定したコアIDと異なる場合
	 */
	if (coreid != id) {
		test_outputlog_2("## Core %d : Wrong CoreID is specified [%d]", coreid, id);
		test_outputlog_2("## in \"test_mc_check_point(%d, %d)\".", id, count);
		errorflag = TRUE;
	}
	else {
		/*
		 *  シーケンスチェック
		 */
		check_count_mc[coreid]++;
		if (check_count_mc[coreid] == count) {
			test_outputlog_2("Core %d : Check point %d passed.", coreid, count);
		}
		else {
			test_outputlog_2("## Core %d : Unexpected Check Point : %d", coreid, count);
			test_outputlog_2("## in \"test_mc_check_point(%d, %d)\".", id, count);
			errorflag = TRUE;
		}
	}

	/* 割込み禁止状態を解除 */
	test_ena_int(locked);

	if (errorflag == TRUE) {
		cp_state = FALSE;
		test_shutdown();
	}
}


/*
 *  完了チェックポイント(マルチコア用)
 */
void
test_mc_check_finish(uint8 id, uint32 count)
{
	test_mc_check_point(id, count);
	if (cp_state == TRUE) {
		test_outputlog_1("Core %d : All check points passed.", id);
	}
	test_shutdown();
}


/*
 *  バリア同期(マルチコア用)
 */
void
test_barrier_sync(uint32 phase, uint16 core_num)
{
	volatile uint32  i;
	volatile uint32  flag;
	CoreIdType coreid;
	uint32   timeout = 0U;

	/* コアID取得 */
	coreid = GetCoreID();
	local_phase[coreid] = phase;

	/* マスターコアの場合 */
	if(coreid == OS_CORE_ID_MASTER) {
		do {
			flag = 0;
			for(i = 0; i < TotalNumberOfCores; i++) {
				if(local_phase[i] == phase) {
					flag++;
				}
			}
			test_busywait();
			timeout++;
			if(timeout > TEST_LOOP_COUNT) {
				goto timeout_exit;
			}
		} while(flag < core_num);
		global_phase = phase;
	}
	/* スレーブコアの場合 */
	else {
		while(global_phase != phase) {
			test_busywait();
			timeout++;
			if(timeout > TEST_LOOP_COUNT) {
				goto timeout_exit;
			}
		}
	}

	return;

  /* 同期待ちでタイムアウトした場合 */
  timeout_exit:
	test_outputlog_2("## Core %d : test_barrier_sync(phase:%d) caused a timeout", coreid, phase);
	test_outputlog_2("## in \"test_barrier_sync(%d, %d)\".", phase, core_num);
	cp_state = FALSE;
	test_shutdown();
}



/*
 *  ビジーウェイト関数
 */
void
test_busywait(void)
{
	volatile uint32 i;

	for (i = 0U; i < TEST_BUSY_LOOP; i++) {
	}
}

#endif  /* TotalNumberOfCores */


/*
 * 割込み禁止状態
 */
boolean
test_dis_int(void)
{
	boolean locked = FALSE;

	/*
	 * DisableAllInterrputによる割込み禁止でなければ，
	 * ネスト可能なSuspendAllInterruptsで割込み禁止とする
	 */
	if (test_get_disallint_state() == FALSE) {
		SuspendAllInterrupts();
		locked = TRUE;
	}
	return(locked);
}


/*
 * 割込み禁止状態解除
 */
void
test_ena_int(boolean locked)
{
	if (locked) {
		ResumeAllInterrupts();
	}
}


/*
 *  シャットダウン処理(SC毎の分岐用)
 */
void
test_shutdown(void)
{
#ifndef TotalNumberOfCores
	ShutdownOS(E_OK);
#else
	ShutdownAllCores(E_OK);
#endif /* TotalNumberOfCores */
}

