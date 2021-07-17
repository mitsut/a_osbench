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
 *  $Id: perf_timer.h 1658 2016-01-21 10:17:23Z fsi-asanoto $
 */

/*
 *  性能評価用タイマードライバ
 */
#ifndef PERF_TIMER_H
#define PERF_TIMER_H

#include "test_implement_lib.h"
#include "test_target_lib.h"
#include "target_hw_counter.h"

/*
 *  TAUB関連レジスタ
 */
#define TAUB_BASE(n)	((uint32) (0xffe30000U + (n * 0x1000U)))
#define TAUBTPS(n)		(TAUB_BASE(n) + 0x240U)
#define TAUBCDR(n, ch)	(TAUB_BASE(n) + (ch * 0x04U))
#define TAUBCNT(n, ch)	(TAUB_BASE(n) + 0x80U + (ch * 0x04U))
#define TAUBCMOR(n, ch)	(TAUB_BASE(n) + 0x2000U + (ch * 0x04U))
#define TAUBCMUR(n, ch)	(TAUB_BASE(n) + 0xC0U + (ch * 0x04U))
#define TAUBTS(n)		(TAUB_BASE(n) + 0x1C4U)
#define TAUBTT(n)		(TAUB_BASE(n) + 0x1C8U)

#define MCU_TAUB_MASK_CK0				((uint16) 0xfff0)
#define MCU_TAUB_CK0_0					((uint16) 0x0000) /* 分周なし */
#define MCU_TAUB00_CMOR					((uint16) 0x0000)
#define MCU_TAUB00_CMUR					((uint8) 0x01)

#define TAUB_INTNO(n, ch)	(n == 0)? ((uint32) ((134 + ch))) : ((uint32) (248 + ch))

/*
 *  TAUBn タイマの動作開始／停止処理
 */
LOCAL_INLINE void
PerfSetTimerStartTAUB(uint8 n, uint8 ch)
{
	/* タイマ開始処理 */
	sil_wrh_mem((void *) TAUBTS(n), (1 << ch));
}

LOCAL_INLINE void
PerfSetTimerStopTAUB(uint8 n, uint8 ch)
{
	/* タイマ停止処理 */
	sil_wrh_mem((void *) TAUBTT(n), (1 << ch));
}

/*
 *  TAUBnハードウェアカウンタ現在ティック値取得
 */
LOCAL_INLINE TickType
PerfGetCurrentTimeTAUB(uint8 n, uint8 ch, TickType maxval)
{
	TickType	count;
	TickType	curr_time = 0U;

	count = (TickType)sil_reh_mem((void *) (TAUBCNT(n, ch)));

	/* ダウンカウンタの為，現在チック値に変換 */
	curr_time = maxval - count;
	curr_time = (curr_time % maxval);

	return(curr_time);
}


/* ********************** フリーランタイマー ********************** */
/*
 *  フリーランタイマーのベースアドレス
 */
/*
 *  使用するタイマーのユニット番号と差分タイマと現在値タイマのチャネル
 */
#define HWC_PTIM_UNIT  0  /* 0 or 1 */
#define HWC_PTIM_ID    2

/*
 *  タイマクロック（単位は[MHz]）
 */
#define PERF_TIMER_CLOCK  40

/*
 *	タイマカウント値(最大)
 */
#define TIM_CYC  ((uint16) 0xfffeU)


/*
 *  タイマの現在値の読出し
 */

/*
 * 時刻取得
 */
LOCAL_INLINE
void
perf_get_100ntime(uint32 *p_time)
{
	*p_time = PerfGetCurrentTimeTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID, TIM_CYC) * 10 / PERF_TIMER_CLOCK;
}


/* ********************** 周期タイマー（割込み用） ********************** */

/* タイマクロック（単位は[MHz]） */
#define INT_TIMER_CLOCK  40

/*
 *	タイマカウント値(1[ms])
 */
#define INT_TIM_CYC  ((uint32) (40000U))

/*
 * 周期タイマーの種類を表すための型定義
 */
#define HWC_ITIM_UNIT  0  /* 0 or 1 */

#ifndef USE_TP_ISR
#define INT_A_TIMER   3
#define INT_B_TIMER   4
#else /* USE_TP_ISR */
#define INT_A_NW_TIMER    3
#define INT_A_W_TIMER     4
#define INT_B_NW_TIMER    5
#define INT_B_W_TIMER     6
#endif /* USE_TP_ISR */

/*
 * 時刻取得
 */
LOCAL_INLINE void
int_get_100ntime(uint32 *p_time, uint32 tim_type)
{
	*p_time = PerfGetCurrentTimeTAUB(HWC_ITIM_UNIT, tim_type, INT_TIM_CYC) * 10 / PERF_TIMER_CLOCK;
}

/*
 * システム周期、タイムウィンドウ評価用タイマ
 */
#ifdef USE_TP_KERNEL

#include "target_tp_timer.h"
#include "target_hw_counter.h"

#define HWC_ITIM2_UNIT  0  /* 0 or 1 */
#define HWC_ITIM2_ID    5
#define INTNO_ACTISR2_E1 TAUB_INTNO(HWC_ITIM2_UNIT, HWC_ITIM2_ID)


/*
 * 時刻取得
 */
LOCAL_INLINE void
int_get_100ntime2(uint32 *p_time)
{
	*p_time = PerfGetCurrentTimeTAUB(HWC_ITIM2_UNIT, HWC_ITIM2_ID, 4000) * 10 / PERF_TIMER_CLOCK;

}

/*
 * タイマ値の取得(タイムウィンドウタイマ用)
 */
LOCAL_INLINE void
twd_get_100ntime(uint32 *p_time)
{
    /* タイムウィンドウは0.001sec */
    /* タイムウィンドウタイマは8MHz */
	*p_time = GetCurrentTimeTAUJ(0, 2, 8000) * 10 / 8;
}

/*
* タイマ値の取得(システム周期タイマ)
 */
LOCAL_INLINE void
syscyc_get_100ntime(uint32 *p_time)
{
	/* システム周期は0.01sec */
    /* システム周期タイマは8MHz */
	*p_time = GetCurrentTimeTAUJ(0, 3, 80000) * 10 / 8;
}

#endif /* USE_TP_KERNEL*/

#endif /* PERF_TIMER_H */
