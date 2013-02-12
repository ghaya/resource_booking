<?php

/**
 * @file
 * Resource administration and settings UI.
 */

/**
 * Generates the resource type editing form.
 */
function resource_type_form($form, &$form_state, $resource_type, $op = 'edit') {

  if ($op == 'clone') {
    $resource_type->label .= ' (cloned)';
    $resource_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $resource_type->label,
    '#description' => t('The human-readable name of this resource type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($resource_type->type) ? $resource_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $resource_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'resource_type_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this resource type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($resource_type->description) ? $resource_type->description : '',
    '#description' => t('Description about the resource type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save resource type'),
    '#weight' => 40,
  );

  if (!$resource_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete resource type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('resource_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing resource_type.
 */
function resource_type_form_submit(&$form, &$form_state) {
  $resource_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  resource_type_save($resource_type);

  // Redirect user back to list of resource types.
  $form_state['redirect'] = 'admin/structure/resource-types';
}

function resource_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/resource-types/' . $form_state['resource_type']->type . '/delete';
}

/**
 * Resource type delete form.
 */
function resource_type_form_delete_confirm($form, &$form_state, $resource_type) {
  $form_state['resource_type'] = $resource_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['resource_type_id'] = array('#type' => 'value', '#value' => entity_id('resource_type', $resource_type));
  return confirm_form($form,
    t('Are you sure you want to delete resource type %title?', array('%title' => entity_label('resource_type', $resource_type))),
    'resource/' . entity_id('resource_type', $resource_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Resource type delete form submit handler.
 */
function resource_type_form_delete_confirm_submit($form, &$form_state) {
  $resource_type = $form_state['resource_type'];
  resource_type_delete($resource_type);

  watchdog('resource_type', '@type: deleted %title.', array('@type' => $resource_type->type, '%title' => $resource_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $resource_type->type, '%title' => $resource_type->label)));

  $form_state['redirect'] = 'admin/structure/resource-types';
}

/**
 * Page to select resource type to add new resource.
 */
function resource_admin_add_page() {
  $items = array();
  foreach (resource_type_get_types() as $resource_type_key => $resource_type) {
    $items[] = l(entity_label('resource_type', $resource_type), 'resource/add/' . $resource_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of resource to create.')));
}

/**
 * Add new resource page callback.
 */
function resource_add($type) {
  $resource_type = resource_type_get_types($type);

  $resource = entity_create('resource', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('resource', $resource_type))));

  $output = drupal_get_form('resource_form', $resource);

  return $output;
}

/**
 * Resource Form.
 */
function resource_form($form, &$form_state, $resource) {
  $form_state['resource'] = $resource;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $resource->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $resource->description,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $resource->uid,
  );

  field_attach_form('resource', $resource, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save resource'),
    '#submit' => $submit + array('resource_form_submit'),
  );

  // Show Delete button if we edit resource.
  $resource_id = entity_id('resource', $resource);
  if (!empty($resource_id) && resource_access('edit', $resource)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('resource_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'resource_form_validate';

  return $form;
}

function resource_form_validate($form, &$form_state) {

}

/**
 * Resource submit handler.
 */
function resource_form_submit($form, &$form_state) {
  $resource = $form_state['resource'];

  entity_form_submit_build_entity('resource', $resource, $form, $form_state);

  resource_save($resource);

  $resource_uri = entity_uri('resource', $resource);

  $form_state['redirect'] = $resource_uri['path'];

  drupal_set_message(t('Resource %title saved.', array('%title' => entity_label('resource', $resource))));
}

function resource_form_submit_delete($form, &$form_state) {
  $resource = $form_state['resource'];
  $resource_uri = entity_uri('resource', $resource);
  $form_state['redirect'] = $resource_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function resource_delete_form($form, &$form_state, $resource) {
  $form_state['resource'] = $resource;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['resource_type_id'] = array('#type' => 'value', '#value' => entity_id('resource', $resource));
  $resource_uri = entity_uri('resource', $resource);
  return confirm_form($form,
    t('Are you sure you want to delete resource %title?', array('%title' => entity_label('resource', $resource))),
    $resource_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function resource_delete_form_submit($form, &$form_state) {
  $resource = $form_state['resource'];
  resource_delete($resource);

  drupal_set_message(t('Resource %title deleted.', array('%title' => entity_label('resource', $resource))));

  $form_state['redirect'] = '<front>';
}
