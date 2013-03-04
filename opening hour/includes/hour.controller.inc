<?php

/**
 * @file
 * Hour controller class.
 */

class HourController extends EntityAPIController {

  public function create(array $values = array()) {
    global $user;
    $values += array(
      'title' => '',
      'description' => '',
      'start_date' => '',
      'end_date' => '',
      'resource_reference_id' => '',
      'opening_hour' => '',
      'closing_hour' => '',
      'exception_hour' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('hour', $entity);
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
      '#entity_type' => 'hour',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    // Make Date themed like default fields.
    $content['start_date'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('Start Date'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'text',
        '#entity_type' => 'hour',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->start_date)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->start_date))
    );

    // Make Date themed like default fields.
    $content['end_date'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('End Date'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'text',
        '#entity_type' => 'hour',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->end_date)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->end_date))
    );


    $content['opening_hour'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('Opening hour'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'integer',
        '#entity_type' => 'hour',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->opening_hour)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->opening_hour))
    );

     $content['closing_hour'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('Closing hour'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'integer',
        '#entity_type' => 'hour',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->closing_hour)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->closing_hour))
    );

       $content['exception_hour'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('Exception hour'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'integer',
        '#entity_type' => 'hour',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->exception_hour)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->exception_hour))
    );


    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

 class HourTypeController extends EntityAPIControllerExportable {
   public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save hour type.
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
 * UI controller for hour type.
 */
class HourTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage hour types.';
    return $items;
  }
}

/**
 * Hour class.
 */
class Hour extends Entity {
  protected function defaultLabel() {
    return $this->title;
  }

  protected function defaultUri() {
    return array('path' => 'hour/' . $this->identifier());
  }
}

/**
 * hour type class.
 */
class HourType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'hour_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}
