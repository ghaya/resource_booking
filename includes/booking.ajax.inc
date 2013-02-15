<?php

/**
 * @file
 * Resource page callbacks.
 */


/**
 * Resource ajax callback.Toggles booking function
 */
function toggle_room($bid, $booked, $mode = NULL) {
  // If the mode is not AJAX, redirect to the normal view of the page.
 if ($mode != 'ajax') {
    drupal_goto('node/1');
  }

 if ($booked==1) {$message="alreadybooked";} else {$message="notbooked";}

  // Send the AJAX commands to the calling page.
  $commands = array();
  $commands[] = ajax_command_alert($message);

  // NOTE: this API changed after the recording of the video
  // "ajax_commands" is now "ajax" for the #type property
  // and the "#ajax_commands"" property should now be "#commands"
  return array(
      '#type' => 'ajax',
      '#commands' => $commands,
  );
}