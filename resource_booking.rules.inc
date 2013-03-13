<?php
/**
 * * Implements hook_rules_action_info()
 *
 */

function resource_booking_rules_action_info() {
 return array(
   /*
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
    ),*/
   'create_booking_units_week' => array(
     'group' => t('Bookings'),
     'label' => t('Create 1 week of booking units'),
   ),
   'delete_booking_units_week' => array(
     'group' => t('Bookings'),
     'label' => t('Delete 1 week of booking units'),
     'parameter' => array (
       'type' => array(
         'type' => 'text',
         'label' => t('Resource type'),
         'options list' => 'rules_resource_type_list',
         'description' => t('Choose all resources or select a resource type'),
         'restriction' => 'input',
       )
     )
   ),
 );
}






function delete_booking_units_week($type) {
 /****
  *A rules action that deletes one week of booking units
 *the week is the one with the oldest date
 */
 //Get the relevant resource types
 if ($type=="all_resources") {
  //If all resources, get all bundle types
  $type_array=array_keys(resource_type_get_types());
 } else{
  //Else just get the type sent
  $type_array=array();
  $type_array[]=$type;
 }

 foreach ($type_array as $type) {
  //Get relevant resources for types
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'resource')
  ->entityCondition("bundle",$type)
  ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();
  if (isset($result['resource'])) {
   $resource_ids = array_keys($result['resource']);
  } else {
   drupal_set_message (t("no resources of type %type were found", array('%type'=>$type)));
   continue;
  }



  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'booking')
  ->propertyCondition("resource", $resource_ids, 'IN');
  $result = $query->execute();


  //Establish start date based on bookings found if any
  if (!isset($result['booking'])) {
   //If no bookings exist got to next resource type
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
  //Set enddate to 1 week from startdate
  $enddate_timestamp = strtotime("+6 day", $startdate_timestamp);
  $enddate_string=date('Ymd', $enddate_timestamp);



  //DO a loop to delete bookings for the correct dates

   for ( $i = $startdate_timestamp; $i <= $enddate_timestamp; $i += 86400 ){
    $loopdate_string=date('Y-m-d H:i:s', $i); //creates a datestring to match bookings with
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'booking')
    ->entityCondition('bundle', 'booking')
    ->propertyCondition("resource", $resource_ids, 'IN')
    ->propertyCondition('date',$loopdate_string)
    ->addMetaData('account', user_load(1));
    $result = $query->execute();
    if (isset($result['booking'])) {
     $ids = array_keys($result['booking']);
    }
    //entity_delete_multiple('booking', $ids);
    dsm($loopdate_string);
    dsm($result);
    dsm($ids);

    $g=entity_delete_multiple('booking', $ids);
    dsm($g);
   }//End loop dates

  drupal_set_message (t("Bookings of type %type were deleted for the week starting %start_date to %end_date", array('%type'=>$type, '%start_date'=>$startdate_string, '%end_date'=>$enddate_string)));
 }//End loop type
}



function create_booking_units_week() {
 /****
  *A rules action that deletes one week of booking units
 *the week is the one with the oldest date
 */
 //Get all resources

 $query = new EntityFieldQuery();
 $query->entityCondition('entity_type', 'resource')
 ->addMetaData('account', user_load(1)); // Run the query as user 1.
 $result = $query->execute();
 if (isset($result['resource'])) {
  $ids = array_keys($result['resource']);
 } else {
  return t("error - no resources found");
 }


 //Get all bookings for resources
 $query = new EntityFieldQuery();
 $query->entityCondition('entity_type', 'booking')
 ->propertyCondition("resource", $ids, 'IN');
 $result = $query->execute();


 //Establish start date based on bookings found if any
 if (!isset($result['booking'])) {
  //If no bookings exist set today as startday
  drupal_set_message (t("error - no bookings found. System must be initialized through the module configuration screen")); return;
 } else {
  //Else retrieve booking with earliest date to use as startdate
  $ids = array_keys($result['booking']);
  $last_booking_id=array();
  $last_booking_id[]=end($ids); //This captures the first booking unit made. IF booking units are made with this module, the first made will be earliest date
  $last_booking = entity_load('booking',($last_booking_id));
  $lastdate_string=$last_booking["$last_booking_id[0]"]->date;//Get date of first booking in string format
  $lastdate_timestamp=strtotime($lastdate_string); //Convert lastdate to timestamp
  $startdate_timestamp=strtotime("+1 day", $lastdate_timestamp); //Set startdate to date of rist booking
  $startdate_string=date('Ymd', $startdate_timestamp);
  //Set enddate to 1 week from startdate
  $enddate_timestamp = strtotime("+6 day", $startdate_timestamp);
  $enddate_string=date('Ymd', $enddate_timestamp);

  //Calling the original batch function doesn't seem to work from rules so I hade to call the individual functions
  _batch_initialize_booking_system_batch($startdate_string, $enddate_string);
   _batch_another_initialize_booking_system_batch($startdate_string, $enddate_string);
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