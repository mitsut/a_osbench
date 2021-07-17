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
 *  $Id: perf_histogram.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *		実行時間分布集計モジュール（historam.c の性能評価バージョン）
 */

#include "test_common_lib.h"
#include "perf_histogram.h"


/*
 *  実行時間分布計測管理ブロックのエリア
 */
HISTCB histcb_table[TNUM_HIST];


/*
 *  実行時間分布計測の初期化
 */
boolean
init_hist(uint8 histid, uint32 maxval, uint16 *histarea)
{
	HISTCB	*p_histcb;
	uint32	i;
	boolean ret = TRUE;

	/* 不正な分布計測IDの場合FALSEを返す */
	if ((histid < TMIN_HISTID) || (histid > TMAX_HISTID)) {
		ret = FALSE;
	}
	else {
		p_histcb = &(histcb_table[histid - TMIN_HISTID]);

		for (i = 0U; i <= maxval; i++) {
			histarea[i] = 0U;
		}
		p_histcb->maxval = maxval;
		p_histcb->histarea = histarea;
		p_histcb->over = 0U;
		p_histcb->under = 0U;
		p_histcb->initialized = TRUE;
	}
	return(ret);
}

/*
 *  実行時間計測の開始
 */
boolean
begin_measure(uint8 histid)
{
	HISTCB	*p_histcb;
	boolean ret = TRUE;

	/* 不正な分布計測IDの場合FALSEを返す */
	if ((histid < TMIN_HISTID) || (histid > TMAX_HISTID)) {
		ret = FALSE;
	}
	else {
		p_histcb = &(histcb_table[histid - TMIN_HISTID]);

		/* 初期化していない場合FALSEを返す */
		if (p_histcb->initialized == FALSE)	{
			ret = FALSE;
		}
		else {
			HIST_BM_HOOK();
			HIST_GET_TIM(&(p_histcb->begin_time));
		}
	}
	return(ret);
}

/*
 *  実行時間計測の終了
 */
boolean
end_measure(uint8 histid)
{
	HISTCB	*p_histcb;
	HISTTIM	end_time;
	uint32	val;
	boolean ret = TRUE;

	HIST_GET_TIM(&end_time);

	/* 不正な分布計測IDの場合FALSEを返す */
	if ((histid < TMIN_HISTID) || (histid > TMAX_HISTID)) {
		ret = FALSE;
	}
	else {
		p_histcb = &(histcb_table[histid - TMIN_HISTID]);

		/* 初期化していない場合FALSEを返す */
		if (p_histcb->initialized == FALSE)	{
			ret = FALSE;
		}
		else {
			val = HIST_CONV_TIM(end_time - p_histcb->begin_time);
			if (val <= p_histcb->maxval) {
				p_histcb->histarea[val]++;
			}
			else if (val <= ((uint32) 0x7fffffff)) {
				p_histcb->over++;
			}
			else {
				p_histcb->under++;
			}
		}
	}
	return(ret);
}

/*
 *  実行時間分布計測の表示
 */
boolean
print_hist(uint8 histid)
{
	HISTCB	*p_histcb;
	uint32	i;
	boolean ret = TRUE;

	/* 不正な分布計測IDの場合FALSEを返す */
	if ((histid < TMIN_HISTID) || (histid > TMAX_HISTID)) {
		ret = FALSE;
	}
	else {
		p_histcb = &(histcb_table[histid - TMIN_HISTID]);

		/* 初期化していない場合FALSEを返す */
		if (p_histcb->initialized == FALSE)	{
			ret = FALSE;
		}
		else {
			for (i = 0U; i <= p_histcb->maxval; i++) {
				if (p_histcb->histarea[i] > 0U) {
#ifdef MEASURE_100_NANO
					test_outputlog_3("%d.%d\t: %d", i / 10, i % 10, p_histcb->histarea[i]);
#elif MEASURE_10_NANO
					test_outputlog_3("%d.%d\t: %d", i / 100, i % 100, p_histcb->histarea[i]);
#else
					test_outputlog_2("%d : %d", i, p_histcb->histarea[i]);
#endif /* MEASURE_100_NANO */
				}
			}
			if (p_histcb->over > 0U) {
				test_outputlog_2("> %d : %d", p_histcb->maxval, p_histcb->over);
			}
			if (p_histcb->under > 0U) {
				test_outputlog_1("> 0x7fffffff : %d", p_histcb->under);
			}
		}
	}

	return(ret);
}


/*
 * 計測結果算出用(最頻値)
 */
uint32
calc_hist_value(uint8 histid, uint8 ret_type)
{
	HISTCB	*p_histcb;
	uint16	local_cnt = 0;
	uint32	i;
	uint32	ret_hist_val = 0;

	/* histid判定 */
	if ((histid > TMIN_HISTID) || (histid < TMAX_HISTID)) {
		p_histcb = &(histcb_table[histid - TMIN_HISTID]);

		for (i = 0; i <= p_histcb->maxval; i++) {
			if (p_histcb->histarea[i] > 0) {
				/* 計測回数判定 */
				if ((local_cnt < p_histcb->histarea[i]) ||
				    ((local_cnt == p_histcb->histarea[i]) && (ret_type != RET_MIN_VALUE))) {
						ret_hist_val = i;
						local_cnt = p_histcb->histarea[i];
				}
			}
		}
	}
	return(ret_hist_val);
}
