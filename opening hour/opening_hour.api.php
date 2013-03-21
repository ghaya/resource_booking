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
 * Acts on opening_hour being loaded from the database.
 *
 * This hook is invoked during $opening_hour loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $opening_hour entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_opening_hour_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $opening_hour is inserted.
 *
 * This hook is invoked after the $opening_hour is inserted into the database.
 *
 * @param OpeningHour $opening_hour
 *   The $opening_hour that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_opening_hour_insert(OpeningHour $opening_hour) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('opening_hour', $opening_hour),
      'extra' => print_r($opening_hour, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $opening_hour being inserted or updated.
 *
 * This hook is invoked before the $opening_hour is saved to the database.
 *
 * @param OpeningHour $opening_hour
 *   The $opening_hour that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_opening_hour_presave(OpeningHour $opening_hour) {
  $opening_hour->name = 'foo';
}

/**
 * Responds to a $opening_hour being updated.
 *
 * This hook is invoked after the $opening_hour has been updated in the database.
 *
 * @param OpeningHour $opening_hour
 *   The $opening_hour that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_opening_hour_update(OpeningHour $opening_hour) {
  db_update('mytable')
    ->fields(array('extra' => print_r($opening_hour, TRUE)))
    ->condition('id', entity_id('opening_hour', $opening_hour))
    ->execute();
}

/**
 * Responds to $opening_hour deletion.
 *
 * This hook is invoked after the $opening_hour has been removed from the database.
 *
 * @param OpeningHour $opening_hour
 *   The $opening_hour that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_opening_hour_delete(OpeningHour $opening_hour) {
  db_delete('mytable')
    ->condition('pid', entity_id('opening_hour', $opening_hour))
    ->execute();
}

/**
 * Act on a opening_hour that is being assembled before rendering.
 *
 * @param $opening_hour
 *   The opening_hour entity.
 * @param $view_mode
 *   The view mode the opening_hour is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $opening_hour->content prior to rendering. The
 * structure of $opening_hour->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_opening_hour_view($opening_hour, $view_mode, $langcode) {
  $opening_hour->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for opening_hours.
 *
 * @param $build
 *   A renderable array representing the opening_hour content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * opening_hour content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the opening_hour rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_opening_hour().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_opening_hour_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on opening_hour_type being loaded from the database.
 *
 * This hook is invoked during opening_hour_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of opening_hour_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_opening_hour_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a opening_hour_type is inserted.
 *
 * This hook is invoked after the opening_hour_type is inserted into the database.
 *
 * @param OpeningHourType $opening_hour_type
 *   The opening_hour_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_opening_hour_type_insert(OpeningHourType $opening_hour_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('opening_hour_type', $opening_hour_type),
      'extra' => print_r($opening_hour_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a opening_hour_type being inserted or updated.
 *
 * This hook is invoked before the opening_hour_type is saved to the database.
 *
 * @param OpeningHourType $opening_hour_type
 *   The opening_hour_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_opening_hour_type_presave(OpeningHourType $opening_hour_type) {
  $opening_hour_type->name = 'foo';
}

/**
 * Responds to a opening_hour_type being updated.
 *
 * This hook is invoked after the opening_hour_type has been updated in the database.
 *
 * @param OpeningHourType $opening_hour_type
 *   The opening_hour_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_opening_hour_type_update(OpeningHourType $opening_hour_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($opening_hour_type, TRUE)))
    ->condition('id', entity_id('opening_hour_type', $opening_hour_type))
    ->execute();
}

/**
 * Responds to opening_hour_type deletion.
 *
 * This hook is invoked after the opening_hour_type has been removed from the database.
 *
 * @param OpeningHourType $opening_hour_type
 *   The opening_hour_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_opening_hour_type_delete(OpeningHourType $opening_hour_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('opening_hour_type', $opening_hour_type))
    ->execute();
}

/**
 * Define default opening_hour_type configurations.
 *
 * @return
 *   An array of default opening_hour_type, keyed by machine names.
 *
 * @see hook_default_opening_hour_type_alter()
 */
function hook_default_opening_hour_type() {
  $defaults['main'] = entity_create('opening_hour_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default opening_hour_type configurations.
 *
 * @param array $defaults
 *   An array of default opening_hour_type, keyed by machine names.
 *
 * @see hook_default_opening_hour_type()
 */
function hook_default_opening_hour_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
