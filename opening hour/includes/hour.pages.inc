<?php

/**
 * @file
 * Hour page callbacks.
 */


/**
 * Hour view callback.
 */
function hour_view($hour) {
  drupal_set_title(entity_label('hour', $hour));
  return entity_view('hour', array(entity_id('hour', $hour) => $hour), 'full');
}
