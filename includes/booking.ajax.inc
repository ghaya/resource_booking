<?php

/**
 * @file
 * Resource page callbacks.
 */


/**
 * Resource ajax callback.Toggles booking function
 */
function booking_toggle($bid, $booked, $title, $mode = NULL) {
  // If the mode is not AJAX, redirect to the normal view of the page.
  if ($mode != 'ajax') {
    drupal_goto('node/1');
  }

  //What does the user want to do? Set booking request based on argument sent with link
  if ($booked==1) {
    $booking_request=0; $request_text="unbooked";
  } else {
    $booking_request=1; $request_text="booked";
  }

  //If the user wants to book, check to see if item is already booked
  $ew = entity_metadata_wrapper('booking',$bid);
  $booking_check=$ew->booked->value();

  if ($booking_request==1 && $booking_check==1) {
    $message="Whoops - Someone else just booked it. Reload the page.";
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
    else {  //Else book or unbook as they request
      $ew->booked->set($booking_request); 
      $ew->save();
      
      $message="OK you $request_text it";
    
      $commands = array();
      $commands[] = ajax_command_alert($message);
      // Replace the old toggle link with a new one
      $id="#".$title;
      $url_reconstruct="'/booking/".$bid."/".$booking_request."/".$title."/toggle/nojs'";
      
      $commands[] = ajax_command_replace($id, "<a class='use-ajax' id=$id href=$url_reconstruct>$request_text</a>");
      return array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
    }


}