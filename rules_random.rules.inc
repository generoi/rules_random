<?php

/**
 * @file rules_random.rules.inc
 * TODO: Enter file description here.
 */

/**
 * Implements hook_rules_action_info().
 */
function rules_random_rules_condition_info() {
  return array(
    'rules_random_limited' => array(
      'label' => t('Limited randomize'),
      'parameter' => array(
        'cid' => array(
          'type' => 'integer',
          'label' => t('Identifier'),
          'description' => t('A unique identifier for this drawing. The limitations are restricted to this id. Must be an integer, for example a node id.'),
        ),
        'value' => array(
          'type' => 'text',
          'label' => t('Database value'),
          'description' => t('The value to store in the database, e.g. an e-mail address.'),
          'required' => FALSE,
        ),
        'probability' => array(
          'type' => 'integer',
          'label' => t('Probability'),
          'description' => t('The probability of being selected, as a percentage without the sign. From 0 (low) to 100 (high)'),
          'required' => TRUE,
        ),
        'hours' => array(
          'type' => 'list<integer>',
          'label' => t('Hours of draw'),
          'options list' => 'rules_random_hours_of_day',
          'description' => t('The hours of the day when the drawing should be actived. Remember to set these apart enough to actually draw a winner.'),
        ),
      ),
      'group' => t('Campaign'),
      'help' => t('Evaluates to TRUE only when randomly selected and daily limit is not reached.'),
      'base' => 'rules_random_condition_random',
    ),
  );
}

/**
 * Generate a list of all hours of the day
 */
function rules_random_hours_of_day() {
  $list = array();
  foreach (range(0, 23) as $hour) {
    $list[$hour] = sprintf('%02d:00', $hour);
  }
  return $list;
}

/**
 * Randomly select a winner while being limited to one single winner between
 * draw hours. If no draw hours were selected, the draw is active throughout
 * the day.
 */
function rules_random_condition_random($cid, $value, $probability, $draw_hours) {
  // Statically cache the result not to randomize more than once per page
  // request.
  $is_selected = &drupal_static(__FUNCTION__);
  if (!isset($is_selected)) {
    $is_active = FALSE;
    $hour = (int) date('H');
    $active_hour = NULL;

    // Find out which hour we're on.
    foreach ($draw_hours as $draw_hour) {
      if ($hour >= $draw_hour) {
        // Stop at the highest number, the array values must be descending.
        $active_hour = $draw_hour;
        break;
      }
    }
    // The current time is before the first draw, no drawing.
    if (is_null($active_hour)) {
      return FALSE;
    }

    // Make sure no requests check for winners while another is already in
    // progress. Without a lock there is a race condition between the
    // db_select() and the db_insert().
    // @TODO benchmark
    if (lock_acquire('rules_random_select')) {
      // The timestamp for today at the active draw hour.
      $timestamp_active_hour = strtotime(date('Y-m-d') . ' ' . $active_hour . ':00');
      $has_winner = db_select('rules_random', 'c')
        ->fields('c', array('id'))
        ->condition('cid', $cid)
        ->condition('created', $timestamp_active_hour, '>')
        ->range(0, 1)
        ->execute()
        ->rowCount();

      // The campaign is active when a winner hasn't been selected.
      $is_active = !$has_winner;

      // If the drawing isn't active, ignore randomization.
      if (!$is_active) {
        $is_selected = FALSE;
      }
      // The drawing is active, continue with the roll.
      else {
        $is_selected = (rand(0, 100) >= (100 - $probability));
        if ($is_selected) {
          db_insert('rules_random')
            ->fields(array(
              'cid' => $cid,
              'value' => $value,
              'created' => time(),
            ))
            ->execute();
          $is_selected = TRUE;
        }
      }
      lock_release('rules_random_select');
    }
  }
  return $is_selected;
}
