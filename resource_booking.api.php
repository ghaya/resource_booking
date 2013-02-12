<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on resource_booking being loaded from the database.
 *
 * This hook is invoked during $resource_booking loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $resource_booking entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_resource_booking_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $resource_booking is inserted.
 *
 * This hook is invoked after the $resource_booking is inserted into the database.
 *
 * @param ResourceBooking $resource_booking
 *   The $resource_booking that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_resource_booking_insert(ResourceBooking $resource_booking) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('resource_booking', $resource_booking),
      'extra' => print_r($resource_booking, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $resource_booking being inserted or updated.
 *
 * This hook is invoked before the $resource_booking is saved to the database.
 *
 * @param ResourceBooking $resource_booking
 *   The $resource_booking that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_resource_booking_presave(ResourceBooking $resource_booking) {
  $resource_booking->name = 'foo';
}

/**
 * Responds to a $resource_booking being updated.
 *
 * This hook is invoked after the $resource_booking has been updated in the database.
 *
 * @param ResourceBooking $resource_booking
 *   The $resource_booking that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_resource_booking_update(ResourceBooking $resource_booking) {
  db_update('mytable')
    ->fields(array('extra' => print_r($resource_booking, TRUE)))
    ->condition('id', entity_id('resource_booking', $resource_booking))
    ->execute();
}

/**
 * Responds to $resource_booking deletion.
 *
 * This hook is invoked after the $resource_booking has been removed from the database.
 *
 * @param ResourceBooking $resource_booking
 *   The $resource_booking that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_resource_booking_delete(ResourceBooking $resource_booking) {
  db_delete('mytable')
    ->condition('pid', entity_id('resource_booking', $resource_booking))
    ->execute();
}

/**
 * Act on a resource_booking that is being assembled before rendering.
 *
 * @param $resource_booking
 *   The resource_booking entity.
 * @param $view_mode
 *   The view mode the resource_booking is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $resource_booking->content prior to rendering. The
 * structure of $resource_booking->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_resource_booking_view($resource_booking, $view_mode, $langcode) {
  $resource_booking->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for resource_bookings.
 *
 * @param $build
 *   A renderable array representing the resource_booking content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * resource_booking content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the resource_booking rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_resource_booking().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_resource_booking_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on resource_booking_type being loaded from the database.
 *
 * This hook is invoked during resource_booking_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of resource_booking_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_resource_booking_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a resource_booking_type is inserted.
 *
 * This hook is invoked after the resource_booking_type is inserted into the database.
 *
 * @param ResourceBookingType $resource_booking_type
 *   The resource_booking_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_resource_booking_type_insert(ResourceBookingType $resource_booking_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('resource_booking_type', $resource_booking_type),
      'extra' => print_r($resource_booking_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a resource_booking_type being inserted or updated.
 *
 * This hook is invoked before the resource_booking_type is saved to the database.
 *
 * @param ResourceBookingType $resource_booking_type
 *   The resource_booking_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_resource_booking_type_presave(ResourceBookingType $resource_booking_type) {
  $resource_booking_type->name = 'foo';
}

/**
 * Responds to a resource_booking_type being updated.
 *
 * This hook is invoked after the resource_booking_type has been updated in the database.
 *
 * @param ResourceBookingType $resource_booking_type
 *   The resource_booking_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_resource_booking_type_update(ResourceBookingType $resource_booking_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($resource_booking_type, TRUE)))
    ->condition('id', entity_id('resource_booking_type', $resource_booking_type))
    ->execute();
}

/**
 * Responds to resource_booking_type deletion.
 *
 * This hook is invoked after the resource_booking_type has been removed from the database.
 *
 * @param ResourceBookingType $resource_booking_type
 *   The resource_booking_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_resource_booking_type_delete(ResourceBookingType $resource_booking_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('resource_booking_type', $resource_booking_type))
    ->execute();
}

/**
 * Define default resource_booking_type configurations.
 *
 * @return
 *   An array of default resource_booking_type, keyed by machine names.
 *
 * @see hook_default_resource_booking_type_alter()
 */
function hook_default_resource_booking_type() {
  $defaults['main'] = entity_create('resource_booking_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default resource_booking_type configurations.
 *
 * @param array $defaults
 *   An array of default resource_booking_type, keyed by machine names.
 *
 * @see hook_default_resource_booking_type()
 */
function hook_default_resource_booking_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
