#include <val_task1/val_task1.h>

// finite state machine name for tackling task-1
FSM(val_task1)
{
  //define the states
  FSM_STATES
  {
    STATE_INIT, // fails --> error
    STATE_DETECT_PANEL_COARSE, // fails --> indicate error and stay transition to error
    STATE_WALK_TO_SEE_PANEL, // fails --> go back, error,
    STATE_DETECT_HANDLE_CENTER, // indicate error and stay
    STATE_DETECT_PANEL_FINE, // fails -->
    STATE_WALK_TO_PANEL, // fails -->
    STATE_FIX_HANDLE_DETECTION, // fails --> TO GRASPING
    STATE_GRASP_PITCH_HANDLE, //fails --> retry, error
    STATE_CORRECT_PITCH, // fails --> go back STATE_DETECT_HANDLE_CENTER
    STATE_GRASP_YAW_HANDLE, // fails --> retry, error
    STATE_CORRECT_YAW, // // fails --> go back STATE_DETECT_HANDLE_CENTER
    STATE_REDETECT_HANDLE, //fails --> error
    STATE_DETECT_FINISH, // fails --> indicate error and stay transition to error
    STATE_WALK_TO_FINISH, // go back
    STATE_END,
    STATE_ERROR,
    STATE_SKIP_TO_CP3
  }

  // give the start state
  FSM_START(STATE_INIT);

  // state machine structure and logic, describe the states tansitions
  FSM_BGN // begin state machine
  {
    FSM_STATE(STATE_INIT)
    {
      // state excecution, call the task
      FSM_CALL_TASK(STATE_INIT)

       // state transitions
       FSM_TRANSITIONS
      {
        // transitions
        FSM_ON_EVENT("/INIT_RETRY", FSM_NEXT(STATE_INIT))
        FSM_ON_EVENT("/REGULAR", FSM_NEXT(STATE_DETECT_PANEL_COARSE))
        FSM_ON_EVENT("/INIT_FAILED", FSM_NEXT(STATE_ERROR))
        FSM_ON_EVENT("/SKIP_CHECKPOINT", FSM_NEXT(STATE_SKIP_TO_CP3))

        // or on condition or next state directly
      }
    }
    FSM_STATE(STATE_DETECT_PANEL_COARSE)
    {
      FSM_CALL_TASK(STATE_DETECT_PANEL_COARSE)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/DETECT_PANEL_RETRY", FSM_NEXT(STATE_DETECT_PANEL_COARSE))
        FSM_ON_EVENT("/DETECTED_PANEL", FSM_NEXT(STATE_WALK_TO_SEE_PANEL))
        FSM_ON_EVENT("/DETECT_PANEL_FAILED", FSM_NEXT(STATE_ERROR))
      }
    }
    FSM_STATE(STATE_WALK_TO_SEE_PANEL)
    {
      FSM_CALL_TASK(STATE_WALK_TO_SEE_PANEL)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/WALK_FAILED", FSM_NEXT(STATE_ERROR))
        FSM_ON_EVENT("/WALK_EXECUTING", FSM_NEXT(STATE_WALK_TO_SEE_PANEL))
        FSM_ON_EVENT("/WALK_RETRY", FSM_NEXT(STATE_DETECT_PANEL_COARSE))
        FSM_ON_EVENT("/REACHED_PANEL", FSM_NEXT(STATE_DETECT_HANDLE_CENTER))
      }
    }
    FSM_STATE(STATE_DETECT_HANDLE_CENTER)
    {
      FSM_CALL_TASK(STATE_DETECT_HANDLE_CENTER)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/DETECT_HANDLE_RETRY", FSM_NEXT(STATE_DETECT_HANDLE_CENTER))
        FSM_ON_EVENT("/DETECT_HANDLE_FAILED", FSM_NEXT(STATE_DETECT_PANEL_COARSE))
        FSM_ON_EVENT("/DETECT_HANDLE_ERROR", FSM_NEXT(STATE_ERROR))
        FSM_ON_EVENT("/DETECTED_HANDLE", FSM_NEXT(STATE_DETECT_PANEL_FINE))
      }
    }
    FSM_STATE(STATE_DETECT_PANEL_FINE)
    {
      FSM_CALL_TASK(STATE_DETECT_PANEL_FINE)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/DETECT_PANEL_FINE_RETRY", FSM_NEXT(STATE_DETECT_PANEL_FINE))
        FSM_ON_EVENT("/DETECTED_PANEL_FINE", FSM_NEXT(STATE_WALK_TO_PANEL))
        FSM_ON_EVENT("/DETECT_PANEL_FINE_FAILED", FSM_NEXT(STATE_ERROR))
      }
    }
    FSM_STATE(STATE_WALK_TO_PANEL)
    {
      FSM_CALL_TASK(STATE_WALK_TO_PANEL)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/WALK_TO_PANEL_FAILED", FSM_NEXT(STATE_ERROR))
        FSM_ON_EVENT("/WALK_TO_PANEL_EXECUTING", FSM_NEXT(STATE_WALK_TO_PANEL))
        FSM_ON_EVENT("/WALK_TO_PANEL_RETRY", FSM_NEXT(STATE_DETECT_PANEL_FINE))
        FSM_ON_EVENT("/REACHED_PANEL_FINE", FSM_NEXT(STATE_FIX_HANDLE_DETECTION))
      }
    }
    FSM_STATE(STATE_FIX_HANDLE_DETECTION)
    {
      FSM_CALL_TASK(STATE_FIX_HANDLE_DETECTION)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/FIX_RETRY", FSM_NEXT(STATE_FIX_HANDLE_DETECTION))
        FSM_ON_EVENT("/FIXED_HANDLE", FSM_NEXT(STATE_GRASP_PITCH_HANDLE))
        FSM_ON_EVENT("/FIX_HANDLE_FAILED", FSM_NEXT(STATE_GRASP_PITCH_HANDLE))
      }
    }
    FSM_STATE(STATE_GRASP_PITCH_HANDLE)
    {

      FSM_CALL_TASK(STATE_GRASP_PITCH_HANDLE)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/GRASP_PITCH_HANDLE_EXECUTING", FSM_NEXT(STATE_GRASP_PITCH_HANDLE))
        FSM_ON_EVENT("/GRASP_PITCH_HANDLE_RETRY", FSM_NEXT(STATE_GRASP_PITCH_HANDLE))
        FSM_ON_EVENT("/GRASP_PITCH_HANDLE_FAILED", FSM_NEXT(STATE_REDETECT_HANDLE))
        FSM_ON_EVENT("/GRASPED_PITCH_HANDLE", FSM_NEXT(STATE_CORRECT_PITCH))
      }
    }
    FSM_STATE(STATE_CORRECT_PITCH)
    {

      FSM_CALL_TASK(STATE_CORRECT_PITCH)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/PITCH_CORRECTION_EXECUTING", FSM_NEXT(STATE_CORRECT_PITCH))
        FSM_ON_EVENT("/PITCH_CORRECTION_RETRY", FSM_NEXT(STATE_REDETECT_HANDLE))
        FSM_ON_EVENT("/PITCH_CORRECTION_SUCESSFUL", FSM_NEXT(STATE_GRASP_YAW_HANDLE))
        FSM_ON_EVENT("/PITCH_CORRECTION_FAILED", FSM_NEXT(STATE_ERROR))
      }
    }
    FSM_STATE(STATE_GRASP_YAW_HANDLE)
    {

      FSM_CALL_TASK(STATE_GRASP_YAW_HANDLE)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/GRASP_YAW_HANDLE_EXECUTING", FSM_NEXT(STATE_GRASP_YAW_HANDLE))
        FSM_ON_EVENT("/GRASP_YAW_HANDLE_RETRY", FSM_NEXT(STATE_GRASP_YAW_HANDLE))
        FSM_ON_EVENT("/GRASP_YAW_HANDLE_FAILED", FSM_NEXT(STATE_REDETECT_HANDLE))
        FSM_ON_EVENT("/GRASPED_YAW_HANDLE", FSM_NEXT(STATE_CORRECT_YAW))
      }
    }
    FSM_STATE(STATE_CORRECT_YAW)
    {

      FSM_CALL_TASK(STATE_CORRECT_YAW)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/YAW_CORRECTION_EXECUTING", FSM_NEXT(STATE_CORRECT_YAW))
        FSM_ON_EVENT("/YAW_CORRECTION_RETRY", FSM_NEXT(STATE_REDETECT_HANDLE))
        FSM_ON_EVENT("/YAW_CORRECTION_SUCESSFUL", FSM_NEXT(STATE_DETECT_FINISH))
        FSM_ON_EVENT("/YAW_CORRECTION_FAILED", FSM_NEXT(STATE_ERROR))
      }
    }
    FSM_STATE(STATE_REDETECT_HANDLE)
    {

      FSM_CALL_TASK(STATE_REDETECT_HANDLE)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/REDETECTED_HANDLE_GRASP_PITCH", FSM_NEXT(STATE_GRASP_PITCH_HANDLE))
        FSM_ON_EVENT("/REDETECTED_HANDLE_GRASP_YAW", FSM_NEXT(STATE_GRASP_YAW_HANDLE))
        FSM_ON_EVENT("/REDETECT_HANDLE_RETRY", FSM_NEXT(STATE_REDETECT_HANDLE))
        FSM_ON_EVENT("/REDETECT_HANDLE_FAILED", FSM_NEXT(STATE_ERROR))
      }
    }
    FSM_STATE(STATE_DETECT_FINISH)
    {

      FSM_CALL_TASK(STATE_DETECT_FINISH)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/DETECT_FINISH_RETRY", FSM_NEXT(STATE_DETECT_FINISH))
        FSM_ON_EVENT("/DETECT_FINISH_FAILED", FSM_NEXT(STATE_ERROR))
        FSM_ON_EVENT("/DETECT_FINISH_SUCESSFUL", FSM_NEXT(STATE_WALK_TO_FINISH))
      }
    }
    FSM_STATE(STATE_WALK_TO_FINISH)
    {

      FSM_CALL_TASK(STATE_WALK_TO_FINISH)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/WALK_TO_FINISH_RETRY", FSM_NEXT(STATE_DETECT_FINISH))
        FSM_ON_EVENT("/WALK_TO_FINISH_EXECUTING", FSM_NEXT(STATE_WALK_TO_FINISH))
        FSM_ON_EVENT("/WALK_TO_FINISH_ERROR", FSM_NEXT(STATE_ERROR))
        FSM_ON_EVENT("/WALK_TO_FINISH_SUCESSFUL", FSM_NEXT(STATE_END))
      }
    }
    FSM_STATE(STATE_END)
    {

      FSM_CALL_TASK(STATE_END)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/END_SUCESSFUL", FSM_NEXT(STATE_END))
      }
    }
    FSM_STATE(STATE_ERROR)
    {

      FSM_CALL_TASK(STATE_ERROR)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/RESTART", FSM_NEXT(STATE_INIT))
      }
    }
    FSM_STATE(STATE_SKIP_TO_CP3)
    {

      FSM_CALL_TASK(STATE_SKIP_TO_CP3)

      FSM_TRANSITIONS
      {
        FSM_ON_EVENT("/SKIPPED_TO_CP3", FSM_NEXT(STATE_DETECT_HANDLE_CENTER))
        FSM_ON_EVENT("/SKIP_CP3_RETRY", FSM_NEXT(STATE_SKIP_TO_CP3))
        FSM_ON_EVENT("/SKIP_CP3_FAILED", FSM_NEXT(STATE_ERROR))
      }
    }
  }
  FSM_END // end of state machine
}
