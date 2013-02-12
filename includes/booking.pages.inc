<?php

/**
 * @file
 * Booking page callbacks.
 */


/**
 * Booking view callback.
 */
function booking_view($booking) {
  drupal_set_title(entity_label('booking', $booking));
  return entity_view('booking', array(entity_id('booking', $booking) => $booking), 'full');
}
