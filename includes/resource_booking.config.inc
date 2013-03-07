<?php

/**
 * @file
 * Module configuration page for resource booking
 */

function resource_booking_config($form_name) {

  return drupal_get_form($form_name);

}

function booking_initialize_form($form, &$form_state) {


  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form to create a new booking system'),
  );
  $form['step1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 1'),
  );
  $form['step1']['resource_type'] = array(
    '#markup' => l('Define a resource type (for example Room)', 'admin/structure/resource-types'),
  );

  $form['step2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 2'),
  );
  $form['step2']['hours'] = array(
    '#markup' => l('Define the hours that your resource type is available', 'opening-hours'),
  );
  $form['step3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 3'),
  );
  $form['step3']['resources'] = array(
    '#markup' => l('Add specific instances of your resource type (for example Room#1)', 'resource/add'),
  );

  $form['step4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 4'),
  );
  $form['step4']['dates'] = array(
    '#type' => 'item',
    '#title' => t('Enter the start and end dates available to create bookable units'),

  );

  $form['step4']['start_date'] = array(
    '#title' => t('Start_date'),
    '#type' => 'textfield',
    '#default_value' => 'yyyymmdd',
    '#description' => t('The start date for creating booking units.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['step4']['end_date'] = array(
    '#title' => t('End_date'),
    '#type' => 'textfield',
    '#default_value' => 'yyyymmdd',
    '#description' => t('The end date for creating booking units.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['step5'] = array(
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
include "var/www/drupal/sites/all/modules/custom/resource_booking/resource_booking.rules.inc";
create_booking_units_date('all_resources', $form_state['values']['start_date'], $form_state['values']['end_date']);
}

function booking_admin_form($form, &$form_state) {


  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form to administer booking system'),
  );

  return $form;
}