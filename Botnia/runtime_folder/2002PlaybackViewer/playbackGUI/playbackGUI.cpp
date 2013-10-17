

//Declare GUI information before defining it
#include "playbackGUI.h"
#include <iostream.h>

#ifdef __cplusplus
extern "C"
{
#endif

//Create a skillz diary of all GUI commands.
	Skillz GUI_Record;

//This method launches GUI
	void launchGUI(char* fileName)
	{
		if (fileName != NULL)
		{
			readSequence(fileName);
		}
		//Initialize diary so we can record GUI actions (like clicking buttons)
		GUI_Record.reset();
		//Start the actual GUI!
		char* argv[3] = {"robocupAI", "PlayBack_GUI.tcl", 0};
		Tk_Main(2, argv, Tcl_AppInit);
	}


//==========================================
//==========================================
//==========================================
	int Tcl_AppInit(Tcl_Interp *interp)
	{
		//-------------------------------
		//Initialize the Tcl/Tk libraries
		//-------------------------------
		if (Tcl_Init(interp) == TCL_ERROR)
			return TCL_ERROR;
		if (Tk_Init(interp) == TCL_ERROR)
			return TCL_ERROR;
		//---------------------------------------
		//What does this do?
		Tcl_StaticPackage(interp, "Tk", Tk_Init, (Tcl_PackageInitProc *) NULL);
		//---------------------------------------
		//Execute the user defined start up script.
		//---------------------------------------------
		//Add command to toggle which team is ours.
		Tcl_CreateCommand(interp,
		                  "toggleTeam_cmd",
		                  toggleTeam_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots position
		Tcl_CreateCommand(interp,
		                  "GetRobotPosition_cmd",
		                  GetRobotPosition_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots debug message
		Tcl_CreateCommand(interp,
		                  "GetRobotMessage_cmd",
		                  GetRobotMessage_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots x position
		Tcl_CreateCommand(interp,
		                  "GetOurRobotX_cmd",
		                  GetOurRobotX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots y position
		Tcl_CreateCommand(interp,
		                  "GetOurRobotY_cmd",
		                  GetOurRobotY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots x position
		Tcl_CreateCommand(interp,
		                  "GetOurRobotRawX_cmd",
		                  GetOurRobotRawX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots y position
		Tcl_CreateCommand(interp,
		                  "GetOurRobotRawY_cmd",
		                  GetOurRobotRawY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots x velocity
		Tcl_CreateCommand(interp,
		                  "GetOurRobotSentXVel_cmd",
		                  GetOurRobotSentXVel_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots y velocity
		Tcl_CreateCommand(interp,
		                  "GetOurRobotSentYVel_cmd",
		                  GetOurRobotSentYVel_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots rotation
		Tcl_CreateCommand(interp,
		                  "GetOurRobotRotation_cmd",
		                  GetOurRobotRotation_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots rotation
		Tcl_CreateCommand(interp,
		                  "GetOurRobotRawRotation_cmd",
		                  GetOurRobotRawRotation_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get a robots last sent y velocity
		Tcl_CreateCommand(interp,
		                  "GetOurRobotSentRotation_cmd",
		                  GetOurRobotSentRotation_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get their robots x position
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotX_cmd",
		                  GetTheirRobotX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get their robots y position
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotY_cmd",
		                  GetTheirRobotY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get their robots x position
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotRawX_cmd",
		                  GetTheirRobotRawX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get their robots y position
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotRawY_cmd",
		                  GetTheirRobotRawY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get their robots x velocity
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotXVel_cmd",
		                  GetTheirRobotXVel_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get their robots y velocity
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotYVel_cmd",
		                  GetTheirRobotYVel_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get ball's x position
		Tcl_CreateCommand(interp,
		                  "GetBallX_cmd",
		                  GetBallX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get ball's y position
		Tcl_CreateCommand(interp,
		                  "GetBallY_cmd",
		                  GetBallY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get ball's x position
		Tcl_CreateCommand(interp,
		                  "GetBallRawX_cmd",
		                  GetBallRawX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get ball's y position
		Tcl_CreateCommand(interp,
		                  "GetBallRawY_cmd",
		                  GetBallRawY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get balls x velocity
		Tcl_CreateCommand(interp,
		                  "GetBallXVel_cmd",
		                  GetBallXVel_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//---------------------------------------------
		//Add command to get balls y velocity
		Tcl_CreateCommand(interp,
		                  "GetBallYVel_cmd",
		                  GetBallYVel_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get balls y velocity
		Tcl_CreateCommand(interp,
		                  "GetCurrentFrame_cmd",
		                  GetCurrentFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if our robot is found
		Tcl_CreateCommand(interp,
		                  "GetOurRobotFound_cmd",
		                  GetOurRobotFound_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if their ball is found
		Tcl_CreateCommand(interp,
		                  "GetTheirRobotFound_cmd",
		                  GetTheirRobotFound_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if a robocup ball is found
		Tcl_CreateCommand(interp,
		                  "GetRoboCupBallFound_cmd",
		                  GetRoboCupBallFound_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if a robot is kicking
		Tcl_CreateCommand(interp,
		                  "GetRobotKick_cmd",
		                  GetRobotKick_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if a robot is dribbling
		Tcl_CreateCommand(interp,
		                  "GetRobotDribble_cmd",
		                  GetRobotDribble_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if a friendly robot has possession
		Tcl_CreateCommand(interp,
		                  "GetFriendlyRobotHasPossesion_cmd",
		                  GetFriendlyRobotHasPossesion_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to find if an opponent robot has possession
		Tcl_CreateCommand(interp,
		                  "GetOpponentRobotHasPossesion_cmd",
		                  GetOpponentRobotHasPossesion_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GetCurrentPlayName_cmd",
		                  GetCurrentPlayName_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "ForwardFrame_cmd",
		                  ForwardFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "BackwardFrame_cmd",
		                  BackwardFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "SetFrame_cmd",
		                  SetFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "NumberOfFrames_cmd",
		                  NumberOfFrames_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "LoadFile_cmd",
		                  LoadFile_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "SaveFile_cmd",
		                  SaveFile_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "AppendFile_cmd",
		                  AppendFile_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GUIMode_cmd",
		                  GUIMode_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GetOurRobotDestinationX_cmd",
		                  GetOurRobotDestinationX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GetOurRobotDestinationY_cmd",
		                  GetOurRobotDestinationY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "GetOurRobotDestinationRot_cmd",
		                  GetOurRobotDestinationRot_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GetOurRobotPassValue_cmd",
		                  GetOurRobotPassValue_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GetOurRobotPassDestinationX_cmd",
		                  GetOurRobotPassDestinationX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//Add command to get the current play name
		Tcl_CreateCommand(interp,
		                  "GetOurRobotPassDestinationY_cmd",
		                  GetOurRobotPassDestinationY_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "SetCropLeft_cmd",
		                  SetCropLeft_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "SetCropRight_cmd",
		                  SetCropRight_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "SetCrop_cmd",
		                  SetCrop_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "GetLeftMostCropIncludingFrame_cmd",
		                  GetLeftMostCropIncludingFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "GetRightMostCropIncludingFrame_cmd",
		                  GetRightMostCropIncludingFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "DeleteAllCropsIncludingFrame_cmd",
		                  DeleteAllCropsIncludingFrame_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		//==================================
		Tcl_CreateCommand(interp,
		                  "LEFT_SIDE_LINE_cmd",
		                  LEFT_SIDE_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "RIGHT_SIDE_LINE_cmd",
		                  RIGHT_SIDE_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "OUR_GOAL_LINE_cmd",
		                  OUR_GOAL_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "THEIR_GOAL_LINE_cmd",
		                  THEIR_GOAL_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "GOAL_DEPTH_cmd",
		                  GOAL_DEPTH_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "LEFT_GOAL_POST_cmd",
		                  LEFT_GOAL_POST_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "RIGHT_GOAL_POST_cmd",
		                  RIGHT_GOAL_POST_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "LEFT_GOALIE_BOX_cmd",
		                  LEFT_GOALIE_BOX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "RIGHT_GOALIE_BOX_cmd",
		                  RIGHT_GOALIE_BOX_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "GOALIE_BOX_DEPTH_cmd",
		                  GOALIE_BOX_DEPTH_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "DEATH_ZONE_LINE_cmd",
		                  DEATH_ZONE_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "DEFENSE_ZONE_LINE_cmd",
		                  DEFENSE_ZONE_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "KILL_ZONE_LINE_cmd",
		                  KILL_ZONE_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "OFFENSE_ZONE_LINE_cmd",
		                  OFFENSE_ZONE_LINE_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "LEFT_MIDDLE_SECTOR_cmd",
		                  LEFT_MIDDLE_SECTOR_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "RIGHT_MIDDLE_SECTOR_cmd",
		                  RIGHT_MIDDLE_SECTOR_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "LEFT_SWEET_SECTOR_cmd",
		                  LEFT_SWEET_SECTOR_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_CreateCommand(interp,
		                  "RIGHT_SWEET_SECTOR_cmd",
		                  RIGHT_SWEET_SECTOR_cmd,
		                  (ClientData) NULL,
		                  (Tcl_CmdDeleteProc *) NULL);
		//==================================
		Tcl_SetVar(interp,
		           "tcl_rcFileName",
		           "~/.wishrc",
		           TCL_GLOBAL_ONLY);
		return TCL_OK;
	}
//==========================================
//==========================================
//==========================================

//=======================
//      ArgError
//=======================
	int ArgError(const char *command, Tcl_Interp *interp)
	{
		/*
		 * Provide an error reporting mechanism. If a command is called
		 * with the wrong number of arguments, we create a suitable
		 * error message and pass back the TCL_ERROR code to show that
		 * the command didn't complete properly.
		 */
		sprintf(result, "Wrong number of arguments for function: %s",
		        command);
		/*
		 * This is the method used for passing results back to the
		 * wish interpreter
		 */
		interp->result = result;
		return TCL_ERROR;
	}

//========================
//     ArgvError
//========================
	int ArgvError(char *command, Tcl_Interp *interp)
	{
		/*
		 * Provide an error reporting mechanism. If a command is called
		 * with the invalid arguments, we create a suitable
		 * error message and pass back the TCL_ERROR code to show that
		 * the command didn't complete properly.
		 */
		sprintf(result, "Invalid %s argument",
		        command);
		/*
		 * This is the method used for passing results back to the
		 * wish interpreter
		 */
		interp->result = result;
		return TCL_ERROR;
	}

//Tcl uses arrays of characters
//to pass around output...
	char result[600];

//End extern...
#ifdef __cplusplus
};
#endif



