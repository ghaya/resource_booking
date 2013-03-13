<?php

/*******************************************************************************
 ****************************** Batch API ****************************
******************************************************************************/


// Our first batch operation.  Note: $context contains Batch API data.
function _batch_initialize_booking_system_batch($hour_type, $startdate, $enddate, &$context) {

 //initialize_resource_booking_system($hour_type, $startdate, $enddate);
dsm ("hello")
 $context['finished'] = 1;

}

/**
 * Our useless second operation, here just to show how that's done.
 */
function _batch_another_initialize_booking_system_batch(&$context) {


 drupal_set_message(variable_get('resource_booking_hour_choice'));
 drupal_set_message("this too");

 $context['finished'] = 1;
}

/**
 * The function called when we finish. Displays a success or error message,
 * but could do anything.
 */
function _batch_initialize_booking_system_batch_finished($success, $results, $operations) {

 if ($success) {
  $message = t('All users have had first and last names processed.');
 }
 else {
  $message = t('Finished with error.');
 }
 drupal_set_message($message);
}


/**
 *  This first function creates the bookable units for all resources, either based
 *  on the 12 hour or 24 hour system depending on what the user has selected
 */
function initialize_resource_booking_system($hour_type, $start_date, $end_date) {
dsm("hello");
/*
 //Convert start dates and end dates to timestamps
 $start_date_timestamp=strtotime($start_date);
 $end_date_timestamp=strtotime($end_date);

 //Get all resources


 //Find the resources that correspond to the resource types
 $query = new EntityFieldQuery();
 $query->entityCondition('entity_type', 'resource')
 ->addMetaData('account', user_load(1)); // Run the query as user 1.
 $result = $query->execute();
 if (isset($result['resource'])) {
  $room_ids = array_keys($result['resource']);
 } else {
  $room_ids = "";
 }

 //If no resources return error message
 if (!$room_ids) {
  drupal_set_message(t("Resources must be defined before booking units can be made"), 'error');
  exit;
 }

 if ($hour_type==0) {
  $first_hour=6;
  $last_hour=24;
 } else {
  $first_hour=1;
  $last_hour=24;
 }

 for ( $i = $start_date_timestamp; $i <= $end_date_timestamp; $i += 86400 ){
  $loop_date=date("Ymd",$i); //A string version of the date for use in the loop starts with the beginning date and goes forward 1 day at a time
  //Check to see if booking units already exist for this date if so abort booking creation process
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'booking')
  ->propertyCondition("date", $loop_date)
  ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();
  if (isset($result['booking'])) {
   drupal_set_message(t('There are already booking units created for %date', array('%date'=>$loop_date)), 'error'); $context['finished'] = 1; return;

  }
  //If no booking units already exist
  for ($h = $first_hour; $h <= $last_hour; $h++) { //loops through every hour and creates a booking unit
   foreach ($room_ids as $room_id) {//Loops through each room id

    $e = entity_create('booking' , array('type' => 'booking'));
    $ew = entity_metadata_wrapper('booking',$e);
    $ew->title->set($loop_date."_".$h."_".$room_id);//Not sure what the title should be yet
    $ew->resource->set("$room_id");
    $ew->date->set($loop_date);
    $ew->starthour->set($h);
    $ew->save();


   } //End loop room
  } //End loop hour
 }  //End date loop
 drupal_set_message (t("booking units created for all resources"));

*/
}

function YOYO(){
   $context['finished'] = 0;
}