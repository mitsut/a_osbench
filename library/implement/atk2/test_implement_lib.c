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
 *  $Id: test_implement_lib.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *  テストライブラリ(実装依存部)
 */

#include "test_common_lib.h"
#include "test_target_lib.h"

#include "t_syslog.h"
#include "sysmod/serial.h"
#include "kernel/task.h"
#include "Os_Lcfg.h"


/*
 *  テストライブラリ初期化(実装依存部)
 */
void test_init_impl_lib(void)
{
#ifndef TotalNumberOfCores
	/* シングルコアの場合，ここで実行 */
	InitSerial();
#endif /* TotalNumberOfCores */
	test_init_target_lib();
}


#ifdef TotalNumberOfCores
#ifdef SLAVE_IDLE_TASK
volatile boolean IdleTaskFlag[TotalNumberOfCores];
#endif /* SLAVE_IDLE_TASK */


/*
 *  各コアで実行するテストライブラリ初期化
 */
void
test_init_impl_lib_eachcore(void)
{
	InitSerial();
	test_init_target_lib_eachcore();
}


/*
 *  指定したCOREがアイドル状態か否か
 */
boolean
test_is_core_idle(uint8 core_id)
{
#ifndef SLAVE_IDLE_TASK
	CCB *p_ccb;

	p_ccb  = get_p_ccb(core_id);

	return (p_ccb->p_runtsk == NULL) ? TRUE : FALSE;
#else  /* SLAVE_IDLE_TASK */
/*
 *  タイムアウトつき，アイドル状態フラグ確認関数
 *  
 *  注1 ：アイドルタスクになる待ち状態のみ利用可能
 *        関数を呼び出す際にフラグを上書きするため，
 *        相手が何度も書き込む状態でなければ使えない．
 *
 *  注2 : 本関数はオーバーヘッド，応答性共に悪い．
 *        本関数を含めて性能評価してはいけない．
 *
 *		戻り値:  アイドルタスク   TRUE
 *               タイムアウト     FALSE
 */
	boolean retval = FALSE;
	volatile uint32 i,j;

	for( i=0; i < 1000; i++ ){
		/* 
		 * 相手コアの性能評価への影響を低減するため 
		 * メモリアクセス頻度を下げる(空ループ)
		 */
		for( j = 0 ; j < 9 ; j++ )
		{
			;
		}
		if(IdleTaskFlag[core_id]){
			IdleTaskFlag[core_id] = FALSE;
			retval = TRUE;
			break;
		}
	}
	return retval;
#endif /* SLAVE_IDLE_TASK */
}
#endif /* TotalNumberOfCores */


/*
 *  全割込み禁止状態取得API
 */
boolean
test_get_disallint_state(void)
{
	boolean	state = FALSE;
#ifdef TotalNumberOfCores
	CCB		*p_ccb = get_my_p_ccb();
	uint16	callevel_stat = p_ccb->callevel_stat;
#endif /* TotalNumberOfCores */

	if (((callevel_stat & TSYS_DISALLINT) != 0U) ||
		((callevel_stat & TSYS_SUSALLINT) != 0U)) {
		state = TRUE;
	}

	return(state);
}


/*
 *  ログ出力(書式指定子0)
 */
void
test_outputlog_0(const char8* Format)
{
	syslog_0(LOG_NOTICE, Format);
}


/*
 *  ログ出力(書式指定子1)
 */
void
test_outputlog_1(const char8* Format, sintptr Arg1)
{
	syslog_1(LOG_NOTICE, Format, Arg1);
}


/*
 *  ログ出力(書式指定子2)
 */
void
test_outputlog_2(const char8* Format, sintptr Arg1, sintptr Arg2)
{
	syslog_2(LOG_NOTICE, Format, Arg1, Arg2);
}


/*
 *  ログ出力(書式指定子3)
 */
void
test_outputlog_3(const char8* Format, sintptr Arg1, sintptr Arg2, sintptr Arg3)
{
	syslog_3(LOG_NOTICE, Format, Arg1, Arg2, Arg3);
}

