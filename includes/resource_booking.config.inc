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
  $form['step1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Resource Type'),
  );
  $form['step1']['resource_type'] = array(
    '#markup' => l('Define a resource type (for example Room)', 'admin/structure/resource-types'),
  );

  $form['step2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Specific Resources'),
  );

   $form['step2']['resources'] = array(
    '#markup' => l('Add specific instances of your resource type (for example Room#1)', 'resource/add'),
  );



  $form['step3'] = array(
    '#type' => 'fieldset',
    '#title' => t('12 hour vs 24 hour system'),
  );
   $form['step3']['hour_choice'] = array(
    '#type' => 'radios',
    '#default_value' => '0',
    '#options' => $hour_choice,
    '#description' => t('If your resources are just bookable during the daytime (6AM to midnight) choose 12 hours, otherwise choose a 24 hour system'),
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
    '#title' => t('Step 5'),
  );








  $form['step5']['scheduler'] = array(
    '#type' => 'item',
    '#title' => t('[TODO: Instructions for setting up scheduled updates of booking units through Rules]'),

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

  //create_booking_units_date('all_resources', $form_state['values']['start_date'], $form_state['values']['end_date']);
 include "booking.batch.inc";
 batch_initialize_booking_system($form_state['values']['hour_choice'], $form_state['values']['start_date'],  $form_state['values']['end_date'] );

}

function booking_admin_form($form, &$form_state) {


  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form to administer booking system'),
  );

  return $form;
}