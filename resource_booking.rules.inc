<?php
/**
 * * Implements hook_rules_action_info()
 *
 */

function resource_booking_rules_action_info() {
  return array(
    'create_booking_units_date' => array(
      'group' => t('Bookings'),
      'label' => t('Create booking units for a date interval'),
      'parameter' => array (

        'type' => array(
          'type' => 'text',
          'label' => t('Resource type'),
          'options list' => 'rules_resource_type_list',
          'description' => t('Choose all resources or select a resource type'),
          'restriction' => 'input',
        ),
        'start_date' => array(
          'type' => 'text',
          'label' => t('Start date'),
          'description' => t('Enter start date in format yyyymmdd'),
        ),
        'end_date' => array(
          'type' => 'text',
          'label' => t('End date'),
          'description' => t('Enter end date in format yyyymmdd'),
        )
      )
    ),
    'delete_booking_units_date' => array(
      'group' => t('Bookings'),
      'label' => t('Delete booking units for a date interval'),
      'parameter' => array (

        'type' => array(
          'type' => 'text',
          'label' => t('Resource type'),
          'options list' => 'rules_resource_type_list',
          'description' => t('Choose all resources or select a resource type'),
          'restriction' => 'input',
        ),
        'start_date' => array(
          'type' => 'text',
          'label' => t('Start date'),
          'description' => t('Enter start date in format yyyymmdd'),
        ),
        'end_date' => array(
          'type' => 'text',
          'label' => t('End date'),
          'description' => t('Enter end date in format yyyymmdd'),
        )
      )
    ),
      'delete_booking_units_week' => array(
      'group' => t('Bookings'),
      'label' => t('Delete x number of weeks of old bookings'),
      'parameter' => array (

        'type' => array(
          'type' => 'text',
          'label' => t('Resource type'),
          'options list' => 'rules_resource_type_list',
          'description' => t('Choose all resources or select a resource type'),
          'restriction' => 'input',
        ),
        'number_of_weeks' => array(
          'type' => 'text',
          'label' => t('Number of weeks'),
          'description' => t('Enter the number of weeks to delete. The earliest bookings created will be deleted'),
        ),
      )
    ),

  );
}



/****
 *A rules action that creates bookings
* based on date parameters
*/
function create_booking_units_date($type, $start_date, $end_date) {
  //Convert start dates and end dates to timestamps
  $start_date_timestamp=strtotime($start_date);
  $end_date_timestamp=strtotime($end_date);

  //Get the relevant resource types
  if ($type=="all_resources") {
    //If all resources, get all bundle types
    $type_array=array_keys(resource_type_get_types());
  } else{
    //Else just get the type sent
    $type_array=array();
    $type_array[]=$type;
  }

  //Loop through each resource type to create booking units for that type
  foreach ($type_array as $type) {

    //Find the resources that correspond to the resource types
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'resource')
    ->entityCondition("bundle",$type)
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
    $result = $query->execute();
    if (isset($result['resource'])) {
      $room_ids = array_keys($result['resource']);
    } else {
      $room_ids = "";
    }

    //If no resources forward to next resource type
    if (!$room_ids) {
      continue;
    }

    //Determine opening and closing hours for this type
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'hour')
    ->entityCondition("bundle", $type);
    $result = $query->execute();

    $hour_ids = array_keys($result['hour']);
    $hours=entity_load("hour", $hour_ids);
    foreach ($hours as $hour) {
      $day_opening_hour=$hour->title."_opening_hour";
      $$day_opening_hour=$hour->opening_hour;  //Create variables for each day t ex $Monday_opening_hour
      $day_closing_hour=$hour->title."_closing_hour";
      $$day_closing_hour=$hour->closing_hour;
    }

    /*
     * Run a loop which goes through the days and creates
    * Booking units for each of the resources
    */

    for ( $i = $start_date_timestamp; $i <= $end_date_timestamp; $i += 86400 ){ //Loops through each date in the date range
      $loop_date=date("Ymd",$i); //A string version of the date for use in the loop starts with the beginning date and goes forward 1 day at a time
      $weekday = date( "l", $i);
      $loop_date_opening_hour=$weekday."_opening_hour";
      $loop_date_closing_hour=$weekday."_closing_hour";
      //Check to see if booking units already exist for this date if so abort booking creation process
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'booking')
      ->propertyCondition("date", $loop_date)
      ->addMetaData('account', user_load(1)); // Run the query as user 1.
      $result = $query->execute();
      if (isset($result['booking'])) {
        drupal_set_message(t('There are already booking units created for %date', array('%date'=>$loop_date)), 'error'); break 2;
      }


      //If no booking units already exist
      for ($h = $$loop_date_opening_hour; $h <= $$loop_date_closing_hour; $h++) { //loops through every hour and creates a booking unit
        if ($$loop_date_opening_hour=="0" || $$loop_date_closing_hour==0) {
          continue;
        }
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
    } //End loop date
    drupal_set_message (t("booking units created for resource type %type", array('%type'=>$type)));
  }//End loop resource


}

/****
 *A rules action that creates bookings
* based on date parameters
*/
function delete_booking_units_date($type, $start_date, $end_date) {
  //Convert start dates and end dates to timestamps
  $start_date_timestamp=strtotime($start_date);
  $end_date_timestamp=strtotime($end_date);


  //Get the relevant resource types
  if ($type=="all_bookings") {
    //If all resources, get all bundle types
    $type_array=array_keys(resource_type_get_types());
  } else{
    //Else just get the type sent
    $type_array=array();
    $type_array[]=$type;
  }

  //Loop through each resource type to create booking units for that type
  foreach ($type_array as $type) {
    for ( $i = $start_date_timestamp; $i <= $end_date_timestamp; $i += 86400 ){ //Loops through each date in the date range
      $loop_date=date("Ymd",$i); //A string version of the date for use in the loop starts with the beginning date and goes forward 1 day at a time

      //Determine opening and closing hours for this type
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'booking')
      ->propertyCondition("date", $loop_date);
      $result = $query->execute();
      if (isset($result['booking'])) {
        $ids = array_keys($result['booking']); entity_delete_multiple("booking", $ids); drupal_set_message(t('Bookings of type %type deleted', array('%type'=>$type))); continue;
      }
    }
  }
}


/****
 *A rules action that deletes bookings based on the number of weeks submitted as parameter
* The earliest bookings created are the ones deleted.
*/
function delete_booking_units_week($type, $number_of_weeks) {
  //Get the relevant resource types
  if ($type=="all_bookings") {
    //If all resources, get all bundle types
    $type_array=array_keys(resource_type_get_types());
  } else{
    //Else just get the type sent
    $type_array=array();
    $type_array[]=$type;
  }



  //Loop through each resource type to create booking units for that type
  foreach ($type_array as $type) {

    //Do a query for the resources of this type
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'resource')
    ->entityCondition("bundle",$type)
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
    $result = $query->execute();
    if (isset($result['resource'])) {
      $ids = array_keys($result['resource']);
    } else {
      continue;
    }

    //Do a query to get the bookings associated with the resource
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'booking')
    ->propertyCondition("resource", $ids, 'IN');
    $result = $query->execute();

    //Establish start date based on bookings found if any
    if (!isset($result['booking'])) {
      //If no bookings exist continue loop
      continue;
    } else {
      //Else retrieve booking with earliest date to use as startdate
      $ids = array_keys($result['booking']);
      $first_booking_id=array();
      $first_booking_id[]=reset($ids); //This captures the first booking unit made. IF booking units are made with this module, the first made will be earliest date
      $first_booking = entity_load('booking',($first_booking_id));
      $firstdate_string=$first_booking["$first_booking_id[0]"]->date;//Get date of first booking in string format
      $firstdate_timestamp=strtotime($firstdate_string); //Convert lastdate to timestamp
      $startdate_timestamp=$firstdate_timestamp; //Set startdate to date of rist booking
      $startdate_string=date('Ymd', $startdate_timestamp);
    }

    //Get the amount of days to create
    $number_of_days=$number_of_weeks*7-1;
    //Set enddate to 1 week from startdate
    $enddate_timestamp = strtotime("$number_of_days day", $startdate_timestamp);
    $enddate_string=date('Ymd', $enddate_timestamp);
    for ( $i = $startdate_timestamp; $i <= $enddate_timestamp; $i += 86400 ){ //Loops through each date in the date range
      $loop_date=date("Ymd",$i); //A string version of the date for use in the loop starts with the beginning date and goes forward 1 day at a time

      //Determine opening and closing hours for this type
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'booking')
      ->propertyCondition("date", $loop_date);
      $result = $query->execute();
      if (isset($result['booking'])) {
        $ids = array_keys($result['booking']); entity_delete_multiple("booking", $ids); drupal_set_message(t('Bookings of type %type deleted', array('%type'=>$type))); continue;
      }
    }

  }
}




//A function that deletes the entire booking system including resources, hours and bookings.  Not sure whether to expose this as a rules action.
function reset_booking_system() {

  $entities = array('resource','resource_type','hour','hour_type', 'booking', 'booking_type');

  foreach ($entities as $entity) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity);
    $result = $query->execute();
    if (isset($result[$entity])) {
      $ids = array_keys($result[$entity]); entity_delete_multiple($entity, $ids); drupal_set_message(t('All entities of type %type deleted', array('%type'=>$entity))); continue;

    }

  }

}


function rules_resource_type_list() {
  $types=array();

  $types=resource_type_get_types();
  $resource_types=array_keys($types);
  array_unshift($resource_types,"all_resources");
  $new_array=array_combine($resource_types,$resource_types);
  return $new_array;


}