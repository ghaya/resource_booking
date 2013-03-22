<?php

/**
 * @file
 * Module configuration page for resource booking
 */

function resource_booking_config($form_name) {

  return drupal_get_form($form_name);

}

function booking_initialize_form($form, &$form_state) {

  // Provide a default date in the format YYYY-MM-DD HH:MM:SS.
  $date = '2013-01-01 00:00:00';
  $format = 'Y-m-d';
  $hour_choice = array(t('12 hour (earliest 6AM latest midnight)'), t('24 hour'));

  $form['description'] = array('#type' => 'item', '#title' => t('A form to create a new booking system'), );

  $form['step2'] = array('#type' => 'fieldset', '#title' => t('Resource Type'), );
  $form['step2']['resource_type'] = array('#markup' => l('Define a resource type (for example Room)', 'admin/structure/resource-types'), );

  $form['step3'] = array('#type' => 'fieldset', '#title' => t('Specific Resources'), );

  $form['step3']['resources'] = array('#markup' => l('Add specific instances of your resource type (for example Room#1)', 'resource/add'), );

  $form['step4'] = array('#type' => 'fieldset', '#title' => t('Hours by Resource Type'), );
  $form['step4']['hours'] = array('#markup' => l('Now define the hours that each resource type is available', 'opening-hours'), );

  $form['step5'] = array('#type' => 'fieldset', '#title' => t('Booking Units'), '#description' => t("The system will now create 'booking units' that are entities for every hour/resource. These units can later be booked by users."));
  $form['step5']['explanation'] = array('#type' => 'item', '#title' => t('The default settings below will set up a system that creates booking units for 6 months in advance from the present date rolling forward, updating the system every week by creating new booking units and deleting old ones.'), );
  $form['step5']['base_weeks'] = array('#title' => t('How many weeks to create?'), '#type' => 'textfield', '#default_value' => '24', '#description' => t('The amount of weeks to create booking units for. Default is 6 months of booking units.'), '#date_year_range' => '0:+1', '#required' => TRUE, '#size' => 30, );
  $form['step5']['add_week'] = array('#title' => t('Create a new week on a rolling basis?'), '#type' => 'checkbox', '#default_value' => '1', '#description' => t('This ensures that you constantly have new weeks to book. So in other words if you choose 6 months of booking units, new units will be added every week so that you always have six months from the present. If you want to administer this manually you can do it through rules.'), );
  $form['step5']['delete_week'] = array('#title' => t('Delete a new week on a rolling basis?'), '#type' => 'checkbox', '#default_value' => '1', '#description' => t('Each week one weeks worth of old bookings are deleted. This is done for performance reasons. Uncheck this if you want to keep a record of old bookings however there may be performance issues if bookings are not deleted. If you want to administer this manually you can do it through rules.'), );

  /*
   $form['step5'] = array(
   '#type' => 'fieldset',
   '#title' => t('Date Range'),
   );
   $form['step5']['dates'] = array(
   '#type' => 'item',
   '#title' => t('Enter the start and end dates available to create bookable units'),
   );
   $form['step5']['start_date'] = array(
   '#title' => t('Start_date'),
   '#type' => 'date_popup',
   '#default_value' => $date,
   '#date_format' => $format,
   '#description' => t('The start date for creating booking units.'),
   '#date_year_range' => '0:+1',
   '#required' => TRUE,
   '#size' => 30,
   );
   $form['step5']['end_date'] = array(
   '#title' => t('End_date'),
   '#type' => 'date_popup',
   '#default_value' => $date,
   '#date_format' => $format,
   '#description' => t('The start date for creating booking units.'),
   '#date_year_range' => '0:+1',
   '#required' => TRUE,
   '#size' => 30,
   );
   */

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Create booking units'), '#weight' => 40, );

  return $form;
}

/**
 * Submit handler for creating/editing resource_type.
 */

function booking_initialize_form_submit(&$form, &$form_state) {

  variable_set('resource_booking_base_weeks', $form_state['values']['base_weeks']);
  
  variable_set('resource_booking_add_week', $form_state['values']['add_week']);
  variable_set('resource_booking_delete_week', $form_state['values']['delete_week']);
  
  $addition=" +".variable_get('resource_booking_base_weeks')." week";
  $startdate_string = date("Y-m-d");
  $enddate= strtotime(date("Y-m-d", strtotime($startdate_string)) . $addition);
  $enddate_string =date("Y-m-d",$enddate);
  //batch_initialize_booking_system($form_state['values']['start_date'],  $form_state['values']['end_date'] );
  batch_initialize_booking_system($startdate_string,  $enddate_string );

}

function booking_initialize_form_validate(&$form, &$form_state) {
  //TODO any validation on initialization form goes here
}

function booking_admin_form($form, &$form_state) {
  $types = resource_type_get_types();
  if (isset($types) && !empty($types)) {
    dsm("hello");
    dsm($types);
    $resource_types = array_keys($types);

    foreach ($resource_types as $type) {
      $max_bookings_label = $type . "_max_bookings";
      $max_bookings_value = variable_get($max_bookings_label);
      $max_advance_label = $type . "_max_advance";
      $max_advance_value = variable_get($max_advance_label);

      $form['description'] = array('#type' => 'item', '#title' => t('A form to administer booking system'), );

      $form[$type] = array('#type' => 'fieldset', '#title' => "$type", );
      $form[$type][$max_bookings_label] = array('#type' => 'textfield', '#title' => t('Max bookings per user'), '#default_value' => $max_bookings_value, '#description' => t('Define the maximum number of hours a user can have.  Only bookings from the present moment onwards count towards the max booking limit, so bookings that have passed already are not counted.'), '#size' => 60, '#maxlength' => 128, '#element_validate' => array('element_validate_number'), );
      $form[$type][$max_advance_label] = array('#type' => 'textfield', '#title' => t('Max days in advance a user can book'), '#default_value' => $max_advance_value, '#description' => t('For example, if you do not want the user to be able to book more than two weeks in advance, enter 14 here'), '#size' => 60, '#maxlength' => 128, '#element_validate' => array('element_validate_number'), );
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Submit'), '#weight' => 40, );
    return $form;

  } else {
    drupal_set_message(t('You have not yet initialized the system'));
  }

}

function booking_admin_form_submit(&$form, &$form_state) {

  $types = resource_type_get_types();
  if (isset($types)) {
    $resource_types = array_keys($types);

    foreach ($resource_types as $type) {
      $max_bookings_label = $type . "_max_bookings";
      $max_advance_label = $type . "_max_advance";

      if (isset($form_state['values'][$max_bookings_label])) {
        variable_set($max_bookings_label, $form_state['values'][$max_bookings_label]);
      }

      if (isset($form_state['values'][$max_advance_label])) {
        variable_set($max_advance_label, $form_state['values'][$max_advance_label]);
      }
    }
  }

  drupal_set_message(t('Booking admin settings saved.'));
}
