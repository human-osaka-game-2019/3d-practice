#ifndef MOVE_H_
#define MOVE_H_

#include "Main.h"

/**
 * @brief  ステップ移動する
 * @param  pThing ステップ移動する3dObject
 */
VOID StepMove(THING*);
/**
 * @brief  sphereの当たり判定の結果をbool返す
 */
VOID MoveCamera();
/**
 * @brief  動かす関数
 */
VOID Move();





#endif // !MOVE_H_
