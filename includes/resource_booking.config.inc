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



 $form['description'] = array(
   '#type' => 'item',
   '#title' => t('A form to create a new booking system'),
 );
 /*
  $form['step1'] = array(
    '#type' => 'fieldset',
    '#title' => t('12 hour vs 24 hour system'),
  );

 $form['step1']['hour_choice'] = array(
   '#type' => 'radios',
   '#default_value' => '0',
   '#options' => $hour_choice,
   '#description' => t('This defines the minimum and maximum bookable hours for the entire system.  You can define specific opening hours per resource type later. If your resources are just going to be bookable during the daytime (6AM to midnight) choose 12 hours, otherwise choose a 24 hour system.'),
 );
 */
 $form['step2'] = array(
   '#type' => 'fieldset',
   '#title' => t('Resource Type'),
 );
 $form['step2']['resource_type'] = array(
   '#markup' => l('Define a resource type (for example Room)', 'admin/structure/resource-types'),
 );

 $form['step3'] = array(
   '#type' => 'fieldset',
   '#title' => t('Specific Resources'),
 );

 $form['step3']['resources'] = array(
   '#markup' => l('Add specific instances of your resource type (for example Room#1)', 'resource/add'),
 );

 $form['step4'] = array(
   '#type' => 'fieldset',
   '#title' => t('Hours by Resource Type'),
 );
 $form['step4']['hours'] = array(
   '#markup' => l('Now define the hours that each resource type is available', 'opening-hours'),
 );

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

 $form['step6'] = array(
   '#type' => 'fieldset',
   '#title' => t('Add/delete booking units on a rolling basis'),
 );
 $form['step6']['scheduler'] = array(
   '#type' => 'item',
   '#description' => t('The module has created Rules actions to add and delete a weeks worth of booking units to your system. You can use rules scheduler to trigger these actions or a regular basis.'),
   '#markup' => l('Rules configuration', 'admin/config/workflow/rules'),
 );


 $form['actions'] = array('#type' => 'actions');
 $form['actions']['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Create booking units'),
   '#weight' => 40,
 );

 return $form;
}

/**
 * Submit handler for creating/editing resource_type.
 */


function booking_initialize_form_submit(&$form, &$form_state) {

 variable_set('resource_booking_hour_choice', 1);
 //batch_initialize_booking_system($form_state['values']['start_date'],  $form_state['values']['end_date'] );

}

function booking_initialize_form_validate(&$form, &$form_state) {
 //TODO any validation on initialization form goes here
}

function booking_admin_form($form, &$form_state) {
 $types=resource_type_get_types();
 if (isset($types) && !empty($types)) {
   dsm("hello"); dsm ($types);
  $resource_types = array_keys($types);


  foreach ($resource_types as $type) {
   $max_bookings_label=$type."_max_bookings";
   $max_bookings_value=variable_get($max_bookings_label);
   $max_advance_label=$type."_max_advance";
   $max_advance_value=variable_get($max_advance_label);


   $form['description'] = array(
     '#type' => 'item',
     '#title' => t('A form to administer booking system'),
   );

   $form[$type] = array(
     '#type' => 'fieldset',
     '#title' => "$type",
   );
   $form[$type][$max_bookings_label] = array(
     '#type' => 'textfield',
     '#title' => t('Max bookings per user'),
     '#default_value' => $max_bookings_value,
     '#description' => t('Define the maximum number of hours a user can have.  Only bookings from the present moment onwards count towards the max booking limit, so bookings that have passed already are not counted.'),
     '#size' => 60,
     '#maxlength' => 128,
     '#element_validate' => array('element_validate_number'),

   );
   $form[$type][$max_advance_label] = array(
     '#type' => 'textfield',
     '#title' => t('Max days in advance a user can book'),
     '#default_value' => $max_advance_value,
     '#description' => t('For example, if you do not want the user to be able to book more than two weeks in advance, enter 14 here'),
     '#size' => 60,
     '#maxlength' => 128,
     '#element_validate' => array('element_validate_number'),

   );
  }

 $form['actions'] = array('#type' => 'actions');
 $form['actions']['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Submit'),
   '#weight' => 40,
 );
 return $form;

 } else {
   drupal_set_message(t('You have not yet initialized the system'));  
 }

 

}

function booking_admin_form_submit(&$form, &$form_state) {

 $types=resource_type_get_types();
 if (isset($types)) {
  $resource_types = array_keys($types);



  foreach ($resource_types as $type) {
   $max_bookings_label=$type."_max_bookings";
   $max_advance_label=$type."_max_advance";

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