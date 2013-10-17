// ************************************************************************************************
//     Copyright 2012 Gao Yuan (robotics-computing.tk)
//     
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
//     Additional license information:
// 
//  **********************************************************************************************/

/**    @file    - move_to_middle_tactic.cpp
  *
  *    @class   - TMoveToMiddle
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 12/20/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#include "move_to_middle_tactic.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"

// set variable as PDataMap
CR_DECLARE(SHOOT_AIM_PREF_AMOUNT)
CR_DECLARE(DISTANCE_FROM_PENALTY_LINE)

inline void cr_do_setup()
{
        CR_SETUP(tactic, SHOOT_AIM_PREF_AMOUNT, CR_DOUBLE);
        CR_SETUP(tactic, DISTANCE_FROM_PENALTY_LINE, CR_DOUBLE);
}

TMoveToMiddle::TMoveToMiddle()
{
}

void TMoveToMiddle::LoadConfig()
{
        cr_do_setup();
}



