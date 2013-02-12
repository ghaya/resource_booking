<?php

/**
 * @file
 * Resource page callbacks.
 */


/**
 * Resource view callback.
 */
function resource_view($resource) {
  drupal_set_title(entity_label('resource', $resource));
  return entity_view('resource', array(entity_id('resource', $resource) => $resource), 'full');
}
