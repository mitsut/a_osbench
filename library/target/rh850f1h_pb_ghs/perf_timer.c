/*
 *  TOPPERS/A-OSBENCH
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Operating System Benchmark
 *
 *  Copyright (C) 2015-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by FUJI SOFT INCORPORATED, JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  $Id: perf_timer.c 1658 2016-01-21 10:17:23Z fsi-asanoto $
 */

/*
 *  評価用タイマードライバ
 */

#include "perf_timer.h"


/* ********************** フリーランタイマー ********************** */

/*
 *  タイマの起動処理
 */
void
perf_timer_initialize(void)
{
	uint16	wk;

	/* 現在値タイマのプリスケーラを設定 PCLK/2^0 */
	wk = sil_reh_mem((void *) TAUBTPS(HWC_PTIM_UNIT));
	wk &= MCU_TAUB_MASK_CK0;
	wk |= MCU_TAUB_CK0_0;
	sil_wrh_mem((void *) TAUBTPS(HWC_PTIM_UNIT), wk);

	/* 現在値タイマ停止処理 */
	PerfSetTimerStopTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID);
	/* 割込み禁止 */
	HwcounterDisableInterrupt(TAUB_INTNO(HWC_PTIM_UNIT, HWC_PTIM_ID));
	/* 割込み要求クリア */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_PTIM_UNIT, HWC_PTIM_ID));

	/* 現在値タイマをインターバルタイマとして設定 */
	sil_wrh_mem((void *) TAUBCMOR(HWC_PTIM_UNIT, HWC_PTIM_ID), MCU_TAUB00_CMOR);
	sil_wrb_mem((void *) TAUBCMUR(HWC_PTIM_UNIT, HWC_PTIM_ID), MCU_TAUB00_CMUR);

	/* タイマカウント周期設定 */
	sil_wrh_mem((void *) TAUBCDR(HWC_PTIM_UNIT, HWC_PTIM_ID), 0xfffe);

	/* 現在値タイマ開始 */
	PerfSetTimerStartTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID);
}


/*
 *  タイマの停止処理
 */
void
perf_timer_terminate(void)
{
	/* 現在値タイマの停止 */
	PerfSetTimerStopTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID);

	/* 現在値タイマの割込み要求のクリア */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_PTIM_UNIT, HWC_PTIM_ID));
}


/* ********************** 周期タイマー（割込み用） ********************** */

/*
 *  タイマの起動処理
 */
void
int_timer_initialize(uint32 tim_type)
{
	uint16	wk;

	/* 現在値タイマ停止処理 */
	PerfSetTimerStopTAUB(HWC_ITIM_UNIT, tim_type);
	/* 割込み禁止 */
	HwcounterDisableInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
	/* 割込み要求クリア */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
	/* 差分タイマの割込み許可 */
	HwcounterEnableInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));

	/* 現在値タイマをインターバルタイマとして設定 */
	sil_wrh_mem((void *) TAUBCMOR(HWC_ITIM_UNIT, tim_type), MCU_TAUB00_CMOR);
	sil_wrb_mem((void *) TAUBCMUR(HWC_ITIM_UNIT, tim_type), MCU_TAUB00_CMUR);

	/* タイマカウント周期設定 */
	sil_wrh_mem((void *) TAUBCDR(HWC_ITIM_UNIT, tim_type), INT_TIM_CYC);

	/* 現在値タイマ開始 */
	PerfSetTimerStartTAUB(HWC_ITIM_UNIT, tim_type);
}


/*
 *  タイマの停止処理
 */
void
int_timer_terminate(uint32 tim_type)
{
	/* 現在値タイマの停止 */
	PerfSetTimerStopTAUB(HWC_ITIM_UNIT, tim_type);

	/* 現在値タイマの割込み禁止 */
	HwcounterDisableInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
	/* 現在値タイマの割込み要求のクリア */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
}


