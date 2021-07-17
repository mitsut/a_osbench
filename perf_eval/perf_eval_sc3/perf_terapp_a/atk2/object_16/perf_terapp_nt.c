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
 *  $Id: perf_terapp_nt.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

#include "perf_terapp_nt.h"


/*
 * *************************************
 * ***** 各処理単位の定義ここから ******
 * *************************************
 */
/* NT_OSAP1 */
TASK(NT_OSAP1_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP1_TerApp_A_Task3);
	ActivateTask(NT_OSAP1_TerApp_A_Task4);
	ActivateTask(NT_OSAP1_TerApp_A_Task5);
	ActivateTask(NT_OSAP1_TerApp_A_Task6);
	ActivateTask(NT_OSAP1_TerApp_A_Task7);
	ActivateTask(NT_OSAP1_TerApp_A_Task8);
	ActivateTask(NT_OSAP1_TerApp_A_Task9);
	ActivateTask(NT_OSAP1_TerApp_A_Task10);
	ActivateTask(NT_OSAP1_TerApp_A_Task11);
	ActivateTask(NT_OSAP1_TerApp_A_Task12);
	ActivateTask(NT_OSAP1_TerApp_A_Task13);
	ActivateTask(NT_OSAP1_TerApp_A_Task14);
	ActivateTask(NT_OSAP1_TerApp_A_Task15);
	ActivateTask(NT_OSAP1_TerApp_A_Task16);
	ActivateTask(NT_OSAP1_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP1, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP1_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP2 */
TASK(NT_OSAP2_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP2_TerApp_A_Task3);
	ActivateTask(NT_OSAP2_TerApp_A_Task4);
	ActivateTask(NT_OSAP2_TerApp_A_Task5);
	ActivateTask(NT_OSAP2_TerApp_A_Task6);
	ActivateTask(NT_OSAP2_TerApp_A_Task7);
	ActivateTask(NT_OSAP2_TerApp_A_Task8);
	ActivateTask(NT_OSAP2_TerApp_A_Task9);
	ActivateTask(NT_OSAP2_TerApp_A_Task10);
	ActivateTask(NT_OSAP2_TerApp_A_Task11);
	ActivateTask(NT_OSAP2_TerApp_A_Task12);
	ActivateTask(NT_OSAP2_TerApp_A_Task13);
	ActivateTask(NT_OSAP2_TerApp_A_Task14);
	ActivateTask(NT_OSAP2_TerApp_A_Task15);
	ActivateTask(NT_OSAP2_TerApp_A_Task16);
	ActivateTask(NT_OSAP2_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP2, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP2_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP3 */
TASK(NT_OSAP3_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP3_TerApp_A_Task3);
	ActivateTask(NT_OSAP3_TerApp_A_Task4);
	ActivateTask(NT_OSAP3_TerApp_A_Task5);
	ActivateTask(NT_OSAP3_TerApp_A_Task6);
	ActivateTask(NT_OSAP3_TerApp_A_Task7);
	ActivateTask(NT_OSAP3_TerApp_A_Task8);
	ActivateTask(NT_OSAP3_TerApp_A_Task9);
	ActivateTask(NT_OSAP3_TerApp_A_Task10);
	ActivateTask(NT_OSAP3_TerApp_A_Task11);
	ActivateTask(NT_OSAP3_TerApp_A_Task12);
	ActivateTask(NT_OSAP3_TerApp_A_Task13);
	ActivateTask(NT_OSAP3_TerApp_A_Task14);
	ActivateTask(NT_OSAP3_TerApp_A_Task15);
	ActivateTask(NT_OSAP3_TerApp_A_Task16);
	ActivateTask(NT_OSAP3_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP3, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP3_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP4 */
TASK(NT_OSAP4_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP4_TerApp_A_Task3);
	ActivateTask(NT_OSAP4_TerApp_A_Task4);
	ActivateTask(NT_OSAP4_TerApp_A_Task5);
	ActivateTask(NT_OSAP4_TerApp_A_Task6);
	ActivateTask(NT_OSAP4_TerApp_A_Task7);
	ActivateTask(NT_OSAP4_TerApp_A_Task8);
	ActivateTask(NT_OSAP4_TerApp_A_Task9);
	ActivateTask(NT_OSAP4_TerApp_A_Task10);
	ActivateTask(NT_OSAP4_TerApp_A_Task11);
	ActivateTask(NT_OSAP4_TerApp_A_Task12);
	ActivateTask(NT_OSAP4_TerApp_A_Task13);
	ActivateTask(NT_OSAP4_TerApp_A_Task14);
	ActivateTask(NT_OSAP4_TerApp_A_Task15);
	ActivateTask(NT_OSAP4_TerApp_A_Task16);
	ActivateTask(NT_OSAP4_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP4, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP4_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP5 */
TASK(NT_OSAP5_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP5_TerApp_A_Task3);
	ActivateTask(NT_OSAP5_TerApp_A_Task4);
	ActivateTask(NT_OSAP5_TerApp_A_Task5);
	ActivateTask(NT_OSAP5_TerApp_A_Task6);
	ActivateTask(NT_OSAP5_TerApp_A_Task7);
	ActivateTask(NT_OSAP5_TerApp_A_Task8);
	ActivateTask(NT_OSAP5_TerApp_A_Task9);
	ActivateTask(NT_OSAP5_TerApp_A_Task10);
	ActivateTask(NT_OSAP5_TerApp_A_Task11);
	ActivateTask(NT_OSAP5_TerApp_A_Task12);
	ActivateTask(NT_OSAP5_TerApp_A_Task13);
	ActivateTask(NT_OSAP5_TerApp_A_Task14);
	ActivateTask(NT_OSAP5_TerApp_A_Task15);
	ActivateTask(NT_OSAP5_TerApp_A_Task16);
	ActivateTask(NT_OSAP5_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP5, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP5_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP6 */
TASK(NT_OSAP6_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP6_TerApp_A_Task3);
	ActivateTask(NT_OSAP6_TerApp_A_Task4);
	ActivateTask(NT_OSAP6_TerApp_A_Task5);
	ActivateTask(NT_OSAP6_TerApp_A_Task6);
	ActivateTask(NT_OSAP6_TerApp_A_Task7);
	ActivateTask(NT_OSAP6_TerApp_A_Task8);
	ActivateTask(NT_OSAP6_TerApp_A_Task9);
	ActivateTask(NT_OSAP6_TerApp_A_Task10);
	ActivateTask(NT_OSAP6_TerApp_A_Task11);
	ActivateTask(NT_OSAP6_TerApp_A_Task12);
	ActivateTask(NT_OSAP6_TerApp_A_Task13);
	ActivateTask(NT_OSAP6_TerApp_A_Task14);
	ActivateTask(NT_OSAP6_TerApp_A_Task15);
	ActivateTask(NT_OSAP6_TerApp_A_Task16);
	ActivateTask(NT_OSAP6_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP6, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP6_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP7 */
TASK(NT_OSAP7_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP7_TerApp_A_Task3);
	ActivateTask(NT_OSAP7_TerApp_A_Task4);
	ActivateTask(NT_OSAP7_TerApp_A_Task5);
	ActivateTask(NT_OSAP7_TerApp_A_Task6);
	ActivateTask(NT_OSAP7_TerApp_A_Task7);
	ActivateTask(NT_OSAP7_TerApp_A_Task8);
	ActivateTask(NT_OSAP7_TerApp_A_Task9);
	ActivateTask(NT_OSAP7_TerApp_A_Task10);
	ActivateTask(NT_OSAP7_TerApp_A_Task11);
	ActivateTask(NT_OSAP7_TerApp_A_Task12);
	ActivateTask(NT_OSAP7_TerApp_A_Task13);
	ActivateTask(NT_OSAP7_TerApp_A_Task14);
	ActivateTask(NT_OSAP7_TerApp_A_Task15);
	ActivateTask(NT_OSAP7_TerApp_A_Task16);
	ActivateTask(NT_OSAP7_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP7, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP7_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP8 */
TASK(NT_OSAP8_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP8_TerApp_A_Task3);
	ActivateTask(NT_OSAP8_TerApp_A_Task4);
	ActivateTask(NT_OSAP8_TerApp_A_Task5);
	ActivateTask(NT_OSAP8_TerApp_A_Task6);
	ActivateTask(NT_OSAP8_TerApp_A_Task7);
	ActivateTask(NT_OSAP8_TerApp_A_Task8);
	ActivateTask(NT_OSAP8_TerApp_A_Task9);
	ActivateTask(NT_OSAP8_TerApp_A_Task10);
	ActivateTask(NT_OSAP8_TerApp_A_Task11);
	ActivateTask(NT_OSAP8_TerApp_A_Task12);
	ActivateTask(NT_OSAP8_TerApp_A_Task13);
	ActivateTask(NT_OSAP8_TerApp_A_Task14);
	ActivateTask(NT_OSAP8_TerApp_A_Task15);
	ActivateTask(NT_OSAP8_TerApp_A_Task16);
	ActivateTask(NT_OSAP8_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP8, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP8_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP9 */
TASK(NT_OSAP9_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP9_TerApp_A_Task3);
	ActivateTask(NT_OSAP9_TerApp_A_Task4);
	ActivateTask(NT_OSAP9_TerApp_A_Task5);
	ActivateTask(NT_OSAP9_TerApp_A_Task6);
	ActivateTask(NT_OSAP9_TerApp_A_Task7);
	ActivateTask(NT_OSAP9_TerApp_A_Task8);
	ActivateTask(NT_OSAP9_TerApp_A_Task9);
	ActivateTask(NT_OSAP9_TerApp_A_Task10);
	ActivateTask(NT_OSAP9_TerApp_A_Task11);
	ActivateTask(NT_OSAP9_TerApp_A_Task12);
	ActivateTask(NT_OSAP9_TerApp_A_Task13);
	ActivateTask(NT_OSAP9_TerApp_A_Task14);
	ActivateTask(NT_OSAP9_TerApp_A_Task15);
	ActivateTask(NT_OSAP9_TerApp_A_Task16);
	ActivateTask(NT_OSAP9_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP9, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task5)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP9_TerApp_A_Task17)
{
	TerminateTask();
}

/* NT_OSAP10 */
TASK(NT_OSAP10_TerApp_A_Task2)
{
	ActivateTask(NT_OSAP10_TerApp_A_Task3);
	ActivateTask(NT_OSAP10_TerApp_A_Task4);
	ActivateTask(NT_OSAP10_TerApp_A_Task5);
	ActivateTask(NT_OSAP10_TerApp_A_Task6);
	ActivateTask(NT_OSAP10_TerApp_A_Task7);
	ActivateTask(NT_OSAP10_TerApp_A_Task8);
	ActivateTask(NT_OSAP10_TerApp_A_Task9);
	ActivateTask(NT_OSAP10_TerApp_A_Task10);
	ActivateTask(NT_OSAP10_TerApp_A_Task11);
	ActivateTask(NT_OSAP10_TerApp_A_Task12);
	ActivateTask(NT_OSAP10_TerApp_A_Task13);
	ActivateTask(NT_OSAP10_TerApp_A_Task14);
	ActivateTask(NT_OSAP10_TerApp_A_Task15);
	ActivateTask(NT_OSAP10_TerApp_A_Task16);
	ActivateTask(NT_OSAP10_TerApp_A_Task17);

	SEQUENCE_CHECK(OS_CORE_ID_MASTER, 3);

	/* TerApp-1 */
	begin_measure(1);
	TerminateApplication(NT_OSAP10, NO_RESTART);

	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task3)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task4)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task5)
{
	TerminateTask();
}


TASK(NT_OSAP10_TerApp_A_Task6)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task7)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task8)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task9)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task10)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task11)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task12)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task13)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task14)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task15)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task16)
{
	TerminateTask();
}

TASK(NT_OSAP10_TerApp_A_Task17)
{
	TerminateTask();
}
