<?php

/**
 * @file
 * Booking administration and settings UI.
 */

/**
 * Generates the booking type editing form.
 */
function booking_type_form($form, &$form_state, $booking_type, $op = 'edit') {

  if ($op == 'clone') {
    $booking_type->label .= ' (cloned)';
    $booking_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $booking_type->label,
    '#description' => t('The human-readable name of this booking type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($booking_type->type) ? $booking_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $booking_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'booking_type_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this booking type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($booking_type->description) ? $booking_type->description : '',
    '#description' => t('Description about the booking type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save booking type'),
    '#weight' => 40,
  );

  if (!$booking_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete booking type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('booking_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing booking_type.
 */
function booking_type_form_submit(&$form, &$form_state) {
  $booking_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  booking_type_save($booking_type);

  // Redirect user back to list of booking types.
  $form_state['redirect'] = 'admin/structure/booking-types';
}

function booking_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/booking-types/' . $form_state['booking_type']->type . '/delete';
}

/**
 * Booking type delete form.
 */
function booking_type_form_delete_confirm($form, &$form_state, $booking_type) {
  $form_state['booking_type'] = $booking_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['booking_type_id'] = array('#type' => 'value', '#value' => entity_id('booking_type', $booking_type));
  return confirm_form($form,
    t('Are you sure you want to delete booking type %title?', array('%title' => entity_label('booking_type', $booking_type))),
    'booking/' . entity_id('booking_type', $booking_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Booking type delete form submit handler.
 */
function booking_type_form_delete_confirm_submit($form, &$form_state) {
  $booking_type = $form_state['booking_type'];
  booking_type_delete($booking_type);

  watchdog('booking_type', '@type: deleted %title.', array('@type' => $booking_type->type, '%title' => $booking_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $booking_type->type, '%title' => $booking_type->label)));

  $form_state['redirect'] = 'admin/structure/booking-types';
}

/**
 * Page to select booking type to add new booking.
 */
function booking_admin_add_page() {
  $items = array();
  foreach (booking_type_get_types() as $booking_type_key => $booking_type) {
    $items[] = l(entity_label('booking_type', $booking_type), 'booking/add/' . $booking_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of booking to create.')));
}

/**
 * Add new booking page callback.
 */
function booking_add($type) {
  $booking_type = booking_type_get_types($type);
  drupal_set_title(t('Create @name', array('@name' => entity_label('booking', $booking_type))));

  $booking = entity_create('booking', array('type' => $type));
  $output = drupal_get_form('booking_form', $booking);

  return $output;
}

/**
 * Booking Form.
 */
function booking_form($form, &$form_state, $booking) {
  $form_state['booking'] = $booking;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $booking->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $booking->description,
  );

  $form['date'] = array(
     '#type' => 'textfield',
      '#title' => t('Date'),
      '#value' => $booking->date,
      '#description' => t('1=booked 0 =notbooked'),
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $booking->uid,
  );

  field_attach_form('booking', $booking, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save booking'),
    '#submit' => $submit + array('booking_form_submit'),
  );

  // Show Delete button if we edit booking.
  $booking_id = entity_id('booking', $booking);
  if (!empty($booking_id) && booking_access('edit', $booking)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('booking_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'booking_form_validate';

  return $form;
}

function booking_form_validate($form, &$form_state) {

}

/**
 * Booking submit handler.
 */
function booking_form_submit($form, &$form_state) {
  $booking = $form_state['booking'];

  entity_form_submit_build_entity('booking', $booking, $form, $form_state);

  booking_save($booking);

  $booking_uri = entity_uri('booking', $booking);

  $form_state['redirect'] = $booking_uri['path'];

  drupal_set_message(t('Booking %title saved.', array('%title' => entity_label('booking', $booking))));
}

function booking_form_submit_delete($form, &$form_state) {
  $booking = $form_state['booking'];
  $booking_uri = entity_uri('booking', $booking);
  $form_state['redirect'] = $booking_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function booking_delete_form($form, &$form_state, $booking) {
  $form_state['booking'] = $booking;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['booking_type_id'] = array('#type' => 'value', '#value' => entity_id('booking', $booking));
  $booking_uri = entity_uri('booking', $booking);
  return confirm_form($form,
    t('Are you sure you want to delete booking %title?', array('%title' => entity_label('booking', $booking))),
    $booking_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function booking_delete_form_submit($form, &$form_state) {
  $booking = $form_state['booking'];
  booking_delete($booking);

  drupal_set_message(t('Booking %title deleted.', array('%title' => entity_label('booking', $booking))));

  $form_state['redirect'] = '<front>';
}
