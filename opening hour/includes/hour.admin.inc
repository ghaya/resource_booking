<?php

/**
 * @file
 * Hour administration and settings UI.
 */

/**
 * Generates the hour type editing form.
 */
function hour_type_form($form, &$form_state, $hour_type, $op = 'edit') {

  if ($op == 'clone') {
    $hour_type->label .= ' (cloned)';
    $hour_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $hour_type->label,
    '#description' => t('The human-readable name of this hour type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($hour_type->type) ? $hour_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $hour_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'hour_type_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this hour type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($hour_type->description) ? $hour_type->description : '',
    '#description' => t('Description about the hour type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save hour type'),
    '#weight' => 40,
  );

  if (!$hour_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete hour type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('hour_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing hour_type.
 */
function hour_type_form_submit(&$form, &$form_state) {
  $hour_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  hour_type_save($hour_type);

  // Redirect user back to list of hour types.
  $form_state['redirect'] = 'admin/structure/hour-types';
}

function hour_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/hour-types/' . $form_state['hour_type']->type . '/delete';
}

/**
 * hour type delete form.
 */
function hour_type_form_delete_confirm($form, &$form_state, $hour_type) {
  $form_state['hour_type'] = $hour_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['hour_type_id'] = array('#type' => 'value', '#value' => entity_id('hour_type', $hour_type));
  return confirm_form($form,
    t('Are you sure you want to delete hour type %title?', array('%title' => entity_label('hour_type', $hour_type))),
    'hour/' . entity_id('hour_type', $hour_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * hour type delete form submit handler.
 */
function hour_type_form_delete_confirm_submit($form, &$form_state) {
  $hour_type = $form_state['hour_type'];
  hour_type_delete($hour_type);

  watchdog('hour_type', '@type: deleted %title.', array('@type' => $hour_type->type, '%title' => $hour_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $hour_type->type, '%title' => $hour_type->label)));

  $form_state['redirect'] = 'admin/structure/hour-types';
}

/**
 * Page to select hour type to add new hour.
 */
function hour_admin_add_page() {
  $items = array();
  foreach (hour_type_get_types() as $hour_type_key => $hour_type) {
    $items[] = l(entity_label('hour_type', $hour_type), 'hour/add/' . $hour_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of hour to create.')));
}

/**
 * Add new hour page callback.
 */
function hour_add($type) {
  $hour_type = hour_type_get_types($type);
  drupal_set_title(t('Create @name', array('@name' => entity_label('hour', $hour_type))));

  $hour = entity_create('hour', array('type' => $type));
  $output = drupal_get_form('hour_form', $hour);

  return $output;
}

/**
 * hour Form.
 */
function hour_form($form, &$form_state, $hour) {
  $form_state['hour'] = $hour;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $hour->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $hour->description,
  );

/*
  $form['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Date'),
    '#default_value' => $hour->start_date,
    '#description' => t('Start date'),
  );
  $form['end_date'] = array(
    '#type' => 'textfield',
    '#title' => t('End Date'),
    '#default_value' => $hour->end_date,
    '#description' => t('End date'),
  );
  $form['resource_reference'] = array(
    '#type' => 'textfield',
    '#title' => t('Resource reference ID'),
    '#default_value' => $hour->resource_reference,
    '#description' => t('ID of corresponding resource'),
  );
  */
  $form['opening_hour'] = array(
    '#type' => 'textfield',
    '#title' => t('Opening hour'),
    '#default_value' => $hour->opening_hour,
    '#description' => t('Hour when resource is first available (24 hour format)'),
  );
  $form['closing_hour'] = array(
    '#type' => 'textfield',
    '#title' => t('Closing hour'),
    '#default_value' => $hour->closing_hour,
    '#description' => t('The hour when the resource is no longer available (24 hour format)'),
  );
  $form['exception_hour'] = array(
    '#type' => 'textfield',
    '#title' => t('Exception hour'),
    '#default_value' => $hour->exception_hour,
    '#description' => t('Hour between opening and closing where resource is not available (24 hour format)'),
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $hour->uid,
  );

  field_attach_form('hour', $hour, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save hour'),
    '#submit' => $submit + array('hour_form_submit'),
  );

  // Show Delete button if we edit hour.
  $hour_id = entity_id('hour', $hour);
  if (!empty($hour_id) && hour_access('edit', $hour)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('hour_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'hour_form_validate';

  return $form;
}

function hour_form_validate($form, &$form_state) {

}

/**
 * hour submit handler.
 */
function hour_form_submit($form, &$form_state) {
  $hour = $form_state['hour'];

  entity_form_submit_build_entity('hour', $hour, $form, $form_state);

  hour_save($hour);

  $hour_uri = entity_uri('hour', $hour);

  $form_state['redirect'] = $hour_uri['path'];

  drupal_set_message(t('Hour %title saved.', array('%title' => entity_label('hour', $hour))));
}

function hour_form_submit_delete($form, &$form_state) {
  $hour = $form_state['hour'];
  $hour_uri = entity_uri('hour', $hour);
  $form_state['redirect'] = $hour_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function hour_delete_form($form, &$form_state, $hour) {
  $form_state['hour'] = $hour;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['hour_type_id'] = array('#type' => 'value', '#value' => entity_id('hour', $hour));
  $hour_uri = entity_uri('hour', $hour);
  return confirm_form($form,
    t('Are you sure you want to delete hour %title?', array('%title' => entity_label('hour', $hour))),
    $hour_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function hour_delete_form_submit($form, &$form_state) {
  $hour = $form_state['hour'];
  hour_delete($hour);

  drupal_set_message(t('hour %title deleted.', array('%title' => entity_label('hour', $hour))));

  $form_state['redirect'] = '<front>';
}
