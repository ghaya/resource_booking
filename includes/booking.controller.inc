<?php

/**
 * @file
 * Booking controller class.
 */

class BookingController extends EntityAPIController {

  public function create(array $values = array()) {
    global $user;
    $values += array(
      'title' => '',
      'description' => '',
      'date' => '',
      'booked' => '',
      'bookable' => '',
      'resource' => '',
      'starthour' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('booking', $entity);
    $content['author'] = array('#markup' => t('Created by: !author', array('!author' => $wrapper->uid->name->value(array('sanitize' => TRUE)))));

    // Make Description themed like default fields.
    $content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Description'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'booking',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    // Make Date themed like default fields.
    $content['date'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('Date'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'text',
        '#entity_type' => 'booking',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->date)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->date))
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

 class BookingTypeController extends EntityAPIControllerExportable {
   public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save Booking type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * UI controller for Booking type.
 */
class BookingTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage booking types.';
    return $items;
  }
}

/**
 * Booking class.
 */
class Booking extends Entity {
  protected function defaultLabel() {
    return $this->title;
  }

  protected function defaultUri() {
    return array('path' => 'booking/' . $this->identifier());
  }
}

/**
 * Booking type class.
 */
class BookingType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'booking_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}
