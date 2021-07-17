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
 *  $Id: test_common_lib.h 646 2016-05-06 05:07:47Z ertl-honda $
 */

#ifndef TEST_COMMON_LIB_H
#define TEST_COMMON_LIB_H

#include "Os.h"


/* チェックポイント初期化 */
extern void test_init_check_cnt(void);

/* テストライブラリ初期化 */
extern void test_init_test_lib(void);

/* 割込み禁止設定 */
extern boolean test_dis_int(void);

/* 割込み禁止解除 */
extern void test_ena_int(boolean locked);

/* システムの終了 */
extern void test_shutdown(void);

/* ビジーウェイト */
extern void test_busywait(void);

/* 実行時間分布計測の初期化 */
extern boolean init_hist(uint8 histid, uint32 maxval, uint16 *histarea);

/* 実行時間計測の開始 */
extern boolean begin_measure(uint8 histid);

/* 実行時間計測の終了 */
extern boolean end_measure(uint8 histid);

/* 実行時間分布計測の表示 */
extern boolean print_hist(uint8 histid);

/* 計測結果算出用(最頻値) */
extern uint32 calc_hist_value(uint8 histid, uint8 ret_type);

/* テストライブラリ初期化時に実装依存で行なうべき処理 */
extern void test_init_impl_lib(void);

/* 全割込み禁止状態の取得 */
extern boolean test_get_disallint_state(void);

/* ログ出力 */
extern void test_outputlog_0(const char8* Format);
extern void test_outputlog_1(const char8* Format, sintptr Arg1);
extern void test_outputlog_2(const char8* Format, sintptr Arg1, sintptr Arg2);
extern void test_outputlog_3(const char8* Format, sintptr Arg1, sintptr Arg2, sintptr Arg3);

/* タイマの起動処理 */
extern void perf_timer_initialize(void);

/* タイマの停止処理 */
extern void perf_timer_terminate(void);

/* タイマの起動処理 */
extern void int_timer_initialize(uint32 type);

/* タイマの停止処理 */
extern void int_timer_terminate(uint32 type);

/* 割込み処理 */
extern void test_int_raise(uint32 intno);

/* 割込み要求のクリア */
extern void test_clear_int_req(uint32 intno);

/* テストライブラリ初期化 */
extern void test_init_target_lib(void);

/* 各コアで実行するテストライブラリ初期化 */
extern void test_init_target_lib_eachcore(void);

/* target_cache_purge */
extern void invalidate_icache(void);

/* CPU例外発生処理 */
extern void raise_cpu_exception(void);

#ifndef TotalNumberOfCores
/* チェックポイントの通過確認 */
extern void test_check_point(uint32 count);
#else
/* 同期フェーズの初期化 */
void test_init_sync_cnt(void);

/* テストライブラリ初期化時に各Coreで行なうべき処理 */
extern void test_init_test_lib_eachcore(void);

/* チェックポイントの通過確認（MC版）*/
extern void test_mc_check_point(uint8 id, uint32 count);

/* バリア同期 */
extern void test_barrier_sync(uint32 phase, uint16 core_num);

/* テストライブラリ初期化時に実装依存で行なうべき処理（MC版）*/
extern void test_init_impl_lib_eachcore(void);

/* コアがIDLE状態であるか判断する関数 */
extern boolean test_is_core_idle(uint8 core_id);
#endif  /* TotalNumberOfCores */

#endif /* TEST_COMMON_LIB_H */
